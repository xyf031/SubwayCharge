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
@ 查询历史消费记录
@ 入参：unCmd, 命令内容
@ 出参: returnStr
@ 返回值: 无
*/
void ProcQueryHistoryCmd(UN_CMD &unCmd, char returnStr[MAX_SEND_BUFFER_LENGTH])
{
    // 查询历史消费记录

    // 查找卡信息 GetCardInfo()
    unsigned int cardNo = unCmd.stCmdQueryHis.cardNo;
    unsigned int balance = 0;
    EN_CARD_TYPE enCard;
    EN_RETURN_CODE enResult = GetCardInfo(cardNo, balance ,enCard);
    if (enResult != EN_RETURN_SUCC)
    {
        GetOutputResultStr(EN_CMD_TYPE_QUERY_HIS, enResult, cardNo, enCard, balance, returnStr);
        return;
    }

    // 判断是否是 单程卡
    if (EN_CARD_TYPE_SINGLE == enCard)
    {
        GetOutputResultStr(EN_CMD_TYPE_QUERY_HIS, EN_RETURN_SINGLE_CARD_CANNOT_HISTORY, cardNo, enCard, balance, returnStr);
        return;
    }

    // 获取历史记录，把结果添加到 returnStr 的后边
    GetOutputResultStr(EN_CMD_TYPE_QUERY_HIS, enResult, cardNo, enCard, balance, returnStr);
    QueryHistory(cardNo, returnStr);
}
