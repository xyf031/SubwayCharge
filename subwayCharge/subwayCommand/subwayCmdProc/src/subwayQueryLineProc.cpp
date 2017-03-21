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
@ 查询地铁线
@ 入参：unCmd, 命令内容
@ 出参: returnStr
@ 返回值: 无
*/
void ProcQueryLineCmd(UN_CMD &unCmd, char returnStr[MAX_SEND_BUFFER_LENGTH])
{
    //查询所有地铁线 GetLineInfo
    string outStr;
    GetLineInfo(outStr);
    const char * cur = outStr.c_str();
    for (unsigned int i = 0; i < strlen(cur); ++i) {
        returnStr[i] = cur[i];
    }
    returnStr[strlen(cur)] = '\0';
}
