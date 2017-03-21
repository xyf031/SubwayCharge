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
@ 办理普通卡
@ 入参：unCmd, 命令内容
@ 出参: returnStr
@ 返回值: 无
*/
void ProcCreateCardCmd(UN_CMD &unCmd, char returnStr[MAX_SEND_BUFFER_LENGTH])
{
    EN_CARD_TYPE enCard;
    //识别卡类型 GetCardType
    unsigned int cardNo = 0;
    unsigned int balance = 0;
    cout << unCmd.stCmdCreateCard.cardType;
    EN_RETURN_CODE returnCode = GetCardType(unCmd.stCmdCreateCard.cardType, enCard);

    if(returnCode != EN_RETURN_SUCC || enCard == EN_CARD_TYPE_SINGLE || enCard == EN_CARD_TYPE_BUTT){
        GetOutputResultStr(EN_CMD_TYPE_CREATE_CARD, EN_RETURN_INPUT_INVALID_CARDTYPE, 
            cardNo, enCard, balance, returnStr);
        return;
    }

    returnCode = AssignCard(cardNo, enCard, unCmd.stCmdCreateCard.cardCharge);
    if(returnCode != EN_RETURN_SUCC){
        GetOutputResultStr(EN_CMD_TYPE_CREATE_CARD, returnCode, cardNo, enCard, balance, returnStr);
        return;
    }

    returnCode = GetCardInfo(cardNo, balance, enCard);
    GetOutputResultStr(EN_CMD_TYPE_CREATE_CARD, returnCode, cardNo, enCard, balance, returnStr);
    return;
}
