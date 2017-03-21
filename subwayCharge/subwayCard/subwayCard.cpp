#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
//#include "list.h"
#include "subwayGlobalDef.h"
//#include "subwayMacro.h"
#include "subwayCard.h"

using namespace std;

//static unsigned int _cardNumber;
static vector<subwayCard*> _subwayCards;

/*
@ 初始化所有卡信息
@ 返回值: 无
*/
void InitCardManagerInfo()
{
    //_cardNumber = 0;  // <- To be deleted.

    int curCardNum = _subwayCards.size();
    for (int i = 0; i < curCardNum; ++i) {
        if (NULL != _subwayCards[i]) {
            free(_subwayCards[i]);
            _subwayCards[i] = NULL;
        } 
    }

    _subwayCards = vector<subwayCard*>(MAX_CARD_NUM, NULL);
}



/*
@ 开卡，分配卡号和卡信息
@ 入参：enCard,卡类型; charge: 充值
@ 出参: cardNo, 分配的卡号
@ 返回值: EN_RETURN_SUCC，分配成功; EN_RETURN_CARD_OVERLOW, 分配失败;
*/
EN_RETURN_CODE AssignCard(unsigned int &cardNo, EN_CARD_TYPE enCard, unsigned int charge)
{
    if (charge > MAX_BALANCE) return EN_RETURN_RECHARGE_OVERFLOW;

    cardNo = MAX_CARD_NUM;
    for (int i = 0; i < MAX_CARD_NUM; ++i) {
        if (NULL == _subwayCards[i]) {
            cardNo = i;
            break;
        }
    }
    if (MAX_CARD_NUM == cardNo) return EN_RETURN_CARD_OVERLOW;
    
    
    subwayCard *pCard = (subwayCard*)malloc(sizeof(subwayCard));
    if (NULL != pCard) {
        pCard->cardNo = cardNo;
        pCard->balance = charge;
        pCard->enCard = enCard;
        pCard->history = NULL;
        _subwayCards[cardNo] = pCard;
    } else
        return EN_RETURN_INNER_ERR;

    return EN_RETURN_SUCC;
}



/*
@ 充值
@ 入参：cardNo,卡号; recharge: 充值
@ 出参: 无
@ 返回值: EN_RETURN_SUCC，成功; 其他, 失败;
*/
EN_RETURN_CODE RechargeCard(unsigned int cardNo, unsigned int recharge)
{

    if (cardNo >= _subwayCards.size() || NULL == _subwayCards[cardNo]) 
        return EN_RETURN_INVALID_CARD;
    if (EN_CARD_TYPE_SINGLE == _subwayCards[cardNo]->enCard) 
        return EN_RETURN_SINGLE_CARD_CANNOT_RECHARGE;
    if ((recharge + _subwayCards[cardNo]->balance) > MAX_BALANCE) 
        return EN_RETURN_RECHARGE_OVERFLOW;

    _subwayCards[cardNo]->balance += recharge;
    return EN_RETURN_SUCC;
}



/*
@ 获取卡余额
@ 入参：cardNo,卡号; 
@ 出参: balance: 余额
        enCard:  卡类型
@ 返回值: EN_RETURN_SUCC，成功; EN_RETURN_INVALID_CARD, 失败;
*/
EN_RETURN_CODE GetCardInfo(unsigned int cardNo, unsigned int &balance, EN_CARD_TYPE &enCard)
{
    if (cardNo >= _subwayCards.size() || NULL == _subwayCards[cardNo]) 
        return EN_RETURN_INVALID_CARD;
    
    balance = _subwayCards[cardNo]->balance;
    enCard = _subwayCards[cardNo]->enCard;
    return EN_RETURN_SUCC;
}



/*
@ 卡扣费
@ 入参：cardNo,卡号; enCard, 卡类型; deductPrice, 扣费票价
@ 出参: balance, 扣费后的余额
@ 返回值: EN_RETURN_SUCC，成功; EN_RETURN_INVALID_CARD, EN_RETURN_BALANCE_NOT_ENOUGH, 失败;
*/
EN_RETURN_CODE DeductCard(unsigned int cardNo, EN_CARD_TYPE enCard, unsigned int deductPrice, unsigned int &balance)
{
    if (cardNo >= _subwayCards.size() || NULL == _subwayCards[cardNo]) 
        return EN_RETURN_INVALID_CARD;
    if(_subwayCards[cardNo]->balance < deductPrice)
        return EN_RETURN_BALANCE_NOT_ENOUGH;

    _subwayCards[cardNo]->balance -= deductPrice;
    balance = _subwayCards[cardNo]->balance;

    return EN_RETURN_SUCC;
}



/*
@ 删除卡信息
@ 入参：cardNo,卡号; 
@ 出参: 无
@ 返回值: 0，成功; -1, 失败;
*/
int DeleteCard(unsigned int cardNo)
{
    if (cardNo >= _subwayCards.size() || NULL == _subwayCards[cardNo])
        return -1;

    // 释放历史记录空间
    pHisNode pHead = _subwayCards[cardNo]->history, pTmp;
    while (pHead != NULL)
    {
        pTmp = pHead->next;
        free(pHead);
        pHead = pTmp;
        if (pHead == _subwayCards[cardNo]->history)
            break;
    }

    free(_subwayCards[cardNo]);
    _subwayCards[cardNo] = NULL;
    return 0;
}



/*
@ 获取卡类型字符串
@ 入参：enCard,卡类型; 
@ 出参: 无
@ 返回值: 卡类型字符串;
*/
char cardNames[][30] = {
    "单程卡",
    "老年卡",
    "普通卡",
    "未知卡类型"
};
char cardName[30] = "未知卡类型";
char* GetCardTypeStr(EN_CARD_TYPE enCard)
{
	switch (enCard)
	{
	case EN_CARD_TYPE_SINGLE: return cardNames[enCard];
	case EN_CARD_TYPE_ELDER: return cardNames[enCard];
	case EN_CARD_TYPE_NOMAL: return cardNames[enCard];
	case EN_CARD_TYPE_BUTT: return cardNames[enCard];
	default: return cardName;
	}
    //return cardNames[enCard];
}
/*
@ 根据卡类型字符串, 识别卡类型
@ 入参：cardType,      卡类型字符串; 
@ 出参: enCard,        卡类型
@ 返回值: EN_RETURN_SUCC，成功; EN_RETURN_INPUT_INVALID_CARDTYPE, 失败;
*/
EN_RETURN_CODE GetCardType(char cardType[], EN_CARD_TYPE &enCard)
{
    if (NULL == cardType) 
        return EN_RETURN_INPUT_INVALID_CARDTYPE;

    switch (cardType[0]) {
        case 'A':
        case 'a': enCard = EN_CARD_TYPE_SINGLE; break;
        case 'B':
        case 'b': enCard = EN_CARD_TYPE_ELDER; break;
        case 'C':
        case 'c': enCard = EN_CARD_TYPE_NOMAL; break;
        default: enCard = EN_CARD_TYPE_BUTT; return EN_RETURN_INPUT_INVALID_CARDTYPE;
    }

    return EN_RETURN_SUCC;
}





/*
@ 插入历史记录
@ 入参：卡号、消费金额、入站时间、入站站名、出站时间、出站站名。
@ 出参：无
@ 返回值：EN_RETURN_SUCC 成功、EN_RETURN_INNER_ERR 内存malloc失败。
*/
EN_RETURN_CODE InsertHistory(unsigned int cardNo, unsigned int deductPrice, ST_SUBWAY_TIME enterTime, ST_SUBWAY_TIME exitTime, const char* enterStation, const char* exitStation)
{
    pHisNode pHistory = (pHisNode) malloc(sizeof(hisNode));
    if (pHistory == NULL)
    {
        return EN_RETURN_INNER_ERR;
    }

    pHistory->enterTime = enterTime;
    pHistory->exitTime = exitTime;
    errno_t ret1 = strcpy_s(pHistory->enterStation, enterStation);
    errno_t ret2 = strcpy_s(pHistory->exitStation, exitStation);
	if (ret1 != ret2) ret1 = ret2;
    pHistory->cost = deductPrice;
    pHistory->next = NULL;

    pHisNode pHead = _subwayCards[cardNo]->history;
    if (pHead == NULL) {
        _subwayCards[cardNo]->history = pHistory;
    } else {
        int historyCount = 1;
        while (pHead->next != NULL)
        {
            historyCount ++;
            if (historyCount >= MAX_HISTORY)
            {
                pHead->next->next = pHistory;
                pHistory->next = _subwayCards[cardNo]->history->next;
                free(_subwayCards[cardNo]->history);
                _subwayCards[cardNo]->history = pHistory->next;
                break;
            }
            pHead = pHead->next;
        }
        if (historyCount < MAX_HISTORY)
        {
            pHead->next = pHistory;
        }
    }

    return EN_RETURN_SUCC;
}



/*
@ 查询历史记录
@ 入参：卡号，待存储字符串
@ 出参：最终输出的字符串
@ 返回值：成功、无效卡
*/
EN_RETURN_CODE QueryHistory(unsigned int cardNo, char returnStr[MAX_SEND_BUFFER_LENGTH])
{
    if (cardNo >= _subwayCards.size() || NULL == _subwayCards[cardNo]) 
        return EN_RETURN_INVALID_CARD;
    
    ostringstream outs;
    pHisNode pHead = _subwayCards[cardNo]->history;
    int count;
    for (count = 1; count <= MAX_HISTORY; ++ count)
    {
        if (pHead == NULL) break;

        outs << "\r\n<序号=" << count;
        outs << ",进站时间=" << pHead->enterTime.hour << ":" << pHead->enterTime.minutes;
        outs << ",进站站点=" << pHead->enterStation;
        outs << ",出站时间=" << pHead->exitTime.hour << ":" << pHead->exitTime.minutes;
        outs << ",出站站点=" << pHead->exitStation;
        outs << ",消费金额=" << pHead->cost;
        outs << ">";
        pHead = pHead->next;
    }

    string out = outs.str();
    if (_subwayCards[cardNo]->history == NULL)
        out = "\r\n<无消费记录>";

    const char* pCopyTmp = out.c_str();
    unsigned int lenFirstLine = strlen(returnStr);
    unsigned int lenHistory = strlen(pCopyTmp);
    //cout << lenFirstLine << " , " << lenHistory << endl;

    unsigned int copyIndex = lenFirstLine;
    for (; copyIndex < lenFirstLine + lenHistory; ++ copyIndex)
    {
        returnStr[copyIndex] = pCopyTmp[copyIndex - lenFirstLine];
    }
    returnStr[lenFirstLine + lenHistory] = '\0';
    //strcpy_s(returnStr, strlen(pCopyTmp), pCopyTmp);
    //strcpy(returnStr, pCopyTmp);

    return EN_RETURN_SUCC;
}
