#ifndef __SUBWAY_CARD_H__
#define __SUBWAY_CARD_H__
#include "subwayError.h"
#include "subwayGlobalDef.h"
#define MAX_STATION_NAME_LENGTH 32


//最多允许系统开100张地铁卡
#define MAX_CARD_NUM 100

//最大余额值
#define MAX_BALANCE 999

//建议最小余额值
#define MIN_BALANCE 20

// 历史记录最大条数
#define MAX_HISTORY 20

//地铁卡类型
typedef enum
{
    EN_CARD_TYPE_SINGLE = 0,
    EN_CARD_TYPE_ELDER,
    EN_CARD_TYPE_NOMAL,
    EN_CARD_TYPE_BUTT
}EN_CARD_TYPE;

// 历史记录
typedef struct hisNode
{
    ST_SUBWAY_TIME enterTime;
    ST_SUBWAY_TIME exitTime;
    char enterStation[MAX_STATION_NAME_LENGTH];
    char exitStation[MAX_STATION_NAME_LENGTH];
    unsigned int cost;
    hisNode* next;
}* pHisNode;

//地铁卡
typedef struct  
{
	unsigned int cardNo;
	unsigned int balance;
	EN_CARD_TYPE enCard;

    pHisNode history;
}subwayCard;




/*
@ 初始化所有卡信息
@ 返回值: 无
*/
void InitCardManagerInfo();

/*
@ 开卡，分配卡号和卡信息
@ 入参：enCard,卡类型; charge: 充值
@ 出参: cardNo, 分配的卡号
@ 返回值: EN_RETURN_SUCC，分配成功; EN_RETURN_CARD_OVERLOW, 分配失败;
*/
EN_RETURN_CODE AssignCard(unsigned int &cardNo, EN_CARD_TYPE enCard, unsigned int charge);

/*
@ 充值
@ 入参：cardNo,卡号; recharge: 充值
@ 出参: 无
@ 返回值: EN_RETURN_SUCC，成功; 其他, 失败;
*/
EN_RETURN_CODE RechargeCard(unsigned int cardNo, unsigned int recharge);

/*
@ 获取卡信息
@ 入参：cardNo,卡号; 
@ 出参: balance: 余额
        enCard:  卡类型
@ 返回值: EN_RETURN_SUCC，成功; EN_RETURN_INVALID_CARD, 失败;
*/
EN_RETURN_CODE GetCardInfo(unsigned int cardNo, unsigned int &balance, EN_CARD_TYPE &enCard);

/*
@ 卡扣费
@ 入参：cardNo,卡号; enCard, 卡类型; deductPrice, 扣费票价
@ 出参: balance, 扣费后的余额;
@ 返回值: EN_RETURN_SUCC，成功; EN_RETURN_INVALID_CARD, EN_RETURN_BALANCE_NOT_ENOUGH, 失败;
*/
EN_RETURN_CODE DeductCard(unsigned int cardNo, EN_CARD_TYPE enCard, unsigned int deductPrice, unsigned int &balance);

/*
@ 根据卡类型字符串, 识别卡类型
@ 入参：cardType,      卡类型字符串; 
@ 出参: enCard,        卡类型
@ 返回值: EN_RETURN_SUCC，成功; EN_RETURN_INPUT_INVALID_CARDTYPE, 失败;
*/
EN_RETURN_CODE GetCardType(char cardType[], EN_CARD_TYPE &enCard);

/*
@ 删除卡信息
@ 入参：cardNo,卡号; 
@ 出参: 无
@ 返回值: 0，成功; -1, 失败;
*/
int DeleteCard(unsigned int cardNo);

/*
@ 获取卡类型字符串
@ 入参：enCard,卡类型; 
@ 出参: 无
@ 返回值: 卡类型字符串，比如"普通卡";
*/
char* GetCardTypeStr(EN_CARD_TYPE enCard);


/*
@ 插入历史记录
@ 入参：卡号、消费金额、入站时间、入站站名、出站时间、出站站名。
@ 出参：无
@ 返回值：EN_RETURN_SUCC 成功、EN_RETURN_INNER_ERR 内存malloc失败。
*/
EN_RETURN_CODE InsertHistory(unsigned int cardNo, unsigned int deductPrice, ST_SUBWAY_TIME enterTime, ST_SUBWAY_TIME exitTime, const char* enterStation, const char* exitStation);


/*
@ 查询历史记录
@ 入参：卡号，待存储字符串
@ 出参：最终输出的字符串
@ 返回值：成功、无效卡
*/
EN_RETURN_CODE QueryHistory(unsigned int cardNo, char returnStr[MAX_SEND_BUFFER_LENGTH]);

#endif
