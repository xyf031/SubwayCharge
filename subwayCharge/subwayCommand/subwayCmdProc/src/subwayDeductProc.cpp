#include "stdafx.h"
#include <iostream>
#include "subwayGlobalDef.h"
#include "subwayMacro.h"
#include "subwayCard.h"
#include "subwayline.h"
#include "subwayPrice.h"
#include "subwayCommon.h"
#include "subwayError.h"
#include "subwayCmdParse/subwayCmdParse.h"
#include "subwayOutput/subwayOutput.h"

using namespace std;

/*
@ 处理扣费命令
@ 入参：stCmdDeduct, 命令内容
@ 出参: returnStr
@ 返回值: void
*/
void ProcDeductCmd(UN_CMD &unCmd, char returnStr[MAX_SEND_BUFFER_LENGTH])
{
    //根据卡号获取卡信息  GetCardInfo
    int cardNo = unCmd.stCmdDeduct.cardNo;
    unsigned int balance = 0;
    EN_CARD_TYPE enCard = EN_CARD_TYPE_BUTT;
    EN_RETURN_CODE returnCode = GetCardInfo(cardNo, balance, enCard);
    if (returnCode != EN_RETURN_SUCC) {
        GetOutputResultStr(EN_CMD_TYPE_DEDUCT, returnCode, cardNo, enCard, balance, returnStr);
        return;
    }

    //检查时间格式  CHECK_TIME
    ST_SUBWAY_TIME enterTime = unCmd.stCmdDeduct.enterTime;
    ST_SUBWAY_TIME exitTime = unCmd.stCmdDeduct.exitTime;
    if (!(CHECK_TIME((&enterTime)) && CHECK_TIME((&exitTime)))) {
        GetOutputResultStr(EN_CMD_TYPE_DEDUCT, EN_RETURN_INVALID_TIME, cardNo, enCard, balance, returnStr);
        return;
    }

    //计算乘车时长  DIFF_TIME
    int diffMinutes = 0;
    DIFF_TIME((&exitTime), (&enterTime), diffMinutes);
    if (diffMinutes < 0) {
        GetOutputResultStr(EN_CMD_TYPE_DEDUCT, EN_RETURN_INVALID_TIME, cardNo, enCard, balance, returnStr);
        return;
    }

    //计算里程数  GetSubwayStationDis
    unsigned int dis = 0;
    returnCode  = GetSubwayStationDis(unCmd.stCmdDeduct.enterStation, unCmd.stCmdDeduct.exitStation, dis);
    if (returnCode != EN_RETURN_SUCC) {
        GetOutputResultStr(EN_CMD_TYPE_DEDUCT, returnCode, cardNo, enCard, balance, returnStr);
        return;
    }

    //计算扣费价格 GetDeductPrice
    unsigned int deductPrice = 0;
    returnCode = GetDeductPrice(enCard, balance, dis, enterTime, exitTime, deductPrice); 
    if (returnCode != EN_RETURN_SUCC) {
        GetOutputResultStr(EN_CMD_TYPE_DEDUCT, returnCode, cardNo, enCard, balance, returnStr);
        return;
    }

    //对乘车卡进行扣费 DeductCard
    returnCode = DeductCard(cardNo, enCard, deductPrice, balance);
    if (returnCode == EN_RETURN_SUCC && enCard == EN_CARD_TYPE_SINGLE) {
        int result = DeleteCard(cardNo);
        if (0 != result) {
            GetOutputResultStr(EN_CMD_TYPE_DEDUCT, EN_RETURN_INNER_ERR, cardNo, enCard, balance, returnStr);
            return;
        }
    } else if (returnCode == EN_RETURN_SUCC && balance < MIN_BALANCE) {
        returnCode = EN_RETURN_BALANCE_TOO_LOW;
    }

    // 插入历史记录
    if (returnCode == EN_RETURN_SUCC && (enCard == EN_CARD_TYPE_ELDER || enCard == EN_CARD_TYPE_NOMAL)) {
        InsertHistory((unsigned int)cardNo, deductPrice, enterTime, exitTime, unCmd.stCmdDeduct.enterStation, unCmd.stCmdDeduct.exitStation);
    }

    //输出字符串
    GetOutputResultStr(EN_CMD_TYPE_DEDUCT, returnCode, cardNo, enCard, balance, returnStr);
    return;
}
