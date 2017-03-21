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
@ 充值命令
@ 入参：unCmd, 命令内容
@ 出参: returnStr
@ 返回值: 无
*/
void ProcFillChargeCmd(UN_CMD &unCmd, char returnStr[MAX_SEND_BUFFER_LENGTH])
{
    //获取卡信息 GetCardInfo
    unsigned int balance;
    unsigned int cardNo = unCmd.stCmdFillCharge.cardNo;
    EN_CARD_TYPE enCard;
    EN_RETURN_CODE enResult = GetCardInfo(cardNo, balance ,enCard);
    if (enResult != EN_RETURN_SUCC)
    {
        GetOutputResultStr(EN_CMD_TYPE_FILL_CHARGE, enResult, cardNo, enCard, balance, returnStr);
        return;
    }


    //进行充值 RechargeCard
    enResult = RechargeCard(cardNo, unCmd.stCmdFillCharge.cardCharge);
    if (enResult != EN_RETURN_SUCC)
    {
        GetOutputResultStr(EN_CMD_TYPE_FILL_CHARGE, enResult, cardNo, enCard, balance, returnStr);
        return;
    }


    //根据充值后的卡信息  GetCardInfo
    enResult = GetCardInfo(cardNo, balance ,enCard);
    GetOutputResultStr(EN_CMD_TYPE_FILL_CHARGE, enResult, cardNo, enCard, balance, returnStr);
    
}