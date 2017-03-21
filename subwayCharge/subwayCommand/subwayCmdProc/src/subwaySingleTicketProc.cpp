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
@ 办理单程票
@ 入参：unCmd, 命令内容
@ 出参: returnStr
@ 返回值: 无
*/

void ProcCreateSingleTicketCmd(UN_CMD &unCmd, char returnStr[MAX_SEND_BUFFER_LENGTH])
{
    //获取两个站点间的里程数 GetSubwayStationDis

    //获取两个站点间的基本票价  GetBasePrice

    //办单程卡 AssignCard

    //输出字符串
    //GetOutputResultStr(EN_CMD_TYPE_SINGLE_TICKET, returnCode, cardNo, EN_CARD_TYPE_SINGLE, basePrice, returnStr);
    char *srcStation = unCmd.stCmdSingleTicket.srcStation;
    char *dstStation = unCmd.stCmdSingleTicket.dstStation;
    unsigned int dis = 0;
    int charge = 0;
    unsigned int cardNo = 0;
    EN_CARD_TYPE enCard = EN_CARD_TYPE_SINGLE;
    EN_RETURN_CODE enResult = GetSubwayStationDis(srcStation, dstStation, dis);
    if (enResult != EN_RETURN_SUCC){
        GetOutputResultStr(EN_CMD_TYPE_SINGLE_TICKET, enResult, cardNo, enCard, charge, returnStr);
        return;
    }
    charge = GetBasePrice(dis);
    enResult = AssignCard(cardNo, enCard, charge);
    GetOutputResultStr(EN_CMD_TYPE_SINGLE_TICKET, enResult, cardNo, enCard, charge, returnStr);
    return;
}