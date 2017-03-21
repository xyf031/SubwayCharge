
#include  <sstream>
#include "subwayMacro.h"
#include "subwayLine.h"
using namespace std;


#ifndef __USE_MULTI_LINES__

// -------------使用单一地铁线路
#include "list.h"


//非法站点号
#define INVALID_STATION_NO (unsigned int)(-1)

//地铁线路方向
typedef enum
{
    EN_DERECT_NEXT = 0,
    EN_DERECT_PREV,
    EN_DERECT_BUTT
}EN_DERECT;

//地铁站
typedef struct
{
    unsigned int no;
    bool usrFlag;
    TListHead list;
    unsigned int dis[EN_DERECT_BUTT];
}ST_STATION;

//地铁线路
typedef struct
{
    TListHead subwayLine;
}ST_LINE;

//地铁站和地铁线列表
ST_STATION g_stasionList[MAX_STATION_NUM];
ST_LINE g_line;


/*
@ 初始化地铁站信息
@ 返回值:void
*/
void InitStation(unsigned int stationNo)
{
    g_stasionList[stationNo].list.next = NULL;
    g_stasionList[stationNo].list.prev = NULL;
    g_stasionList[stationNo].dis[EN_DERECT_NEXT] = 0;
    g_stasionList[stationNo].dis[EN_DERECT_PREV] = 0;
    g_stasionList[stationNo].usrFlag = false;
    g_stasionList[stationNo].no = stationNo;
}

/*
@ 初始化地铁线路信息
@ 返回值:void
*/
void InitSubwayLine()
{
    for(int i = 0; i < MAX_STATION_NUM; i++)
    {
        InitStation(i);
    }

    InitListHead(&(g_line.subwayLine));
}

/*
@ 增加一个地铁站
@ 返回值: -1, 失败;  0, 成功
*/
EN_RETURN_CODE AddSubwayStation(char* stationName, unsigned int dis)
{
    unsigned int stationNo = atoi(&(stationName[1]));

    IF_INVALID_RETURN(stationNo < MAX_STATION_NUM, EN_RETURN_INVALID_STATION)
    IF_INVALID_RETURN(!(g_stasionList[stationNo].usrFlag), EN_RETURN_ADD_STATION_REPEAT)
    IF_INVALID_RETURN(dis <= MAX_DIS_BETWEEN_STATION, EN_RETURN_INVALID_DIS)

    if(ListEmpty(&(g_line.subwayLine)))
    {
        dis = 0;
    }
    else
    {
        IF_INVALID_RETURN(dis != 0, EN_RETURN_INVALID_DIS)

        //把反方向的距离填写成功
        TListHead* prevNode = g_line.subwayLine.prev;
        ST_STATION* prevStation = CONTAINER_OF(prevNode, ST_STATION, list);
        prevStation->dis[EN_DERECT_PREV] = dis;
    }

    //填写当前站点
    g_stasionList[stationNo].usrFlag = true;
    g_stasionList[stationNo].dis[EN_DERECT_PREV] = 0;
    g_stasionList[stationNo].dis[EN_DERECT_NEXT] = dis;  

    ListAddTail(&(g_stasionList[stationNo].list), &(g_line.subwayLine));
    return EN_RETURN_SUCC;
}

/*
@ 根据方向获取下一节点
@ 返回值: TListHead* tmpNode
*/
TListHead* GetNextNode(TListHead* srcNode, EN_DERECT enDerect)
{
    TListHead* tmpNode = NULL;

    if(EN_DERECT_NEXT == enDerect)
    {
        tmpNode = srcNode->next;
    }
    else
    {
        tmpNode = srcNode->prev;
    }
    return tmpNode;
}

/*
@ 朝固定方向查找目标站点
@ 返回值: 0: 成功, -1: 失败
*/
int GetOneDerectDis(TListHead* srcNode, EN_DERECT enDerect, unsigned int stationNo2, unsigned int &dis)
{
    TListHead* tmpNode = NULL;
    ST_STATION* tmpStation = NULL;
    tmpNode = GetNextNode(srcNode, enDerect);
    while((NULL != tmpNode) && ((&(g_line.subwayLine)) != tmpNode))
    {
        tmpStation = CONTAINER_OF(tmpNode, ST_STATION, list);
        dis += tmpStation->dis[enDerect];
        if(tmpStation->no == stationNo2)
        {
            return 0;
        }

        tmpNode = GetNextNode(tmpNode, enDerect);
    }
    return -1;
}

/*
@ 获取两个地铁站间的最小距离
@ 返回值: EN_RETURN_SUCC: 成功, 其他: 失败
*/
EN_RETURN_CODE GetSubwayStationDis(char* stationName1, char* stationName2, unsigned int &dis)
{
    unsigned int stationNo1 = atoi(&(stationName1[1]));
    unsigned int stationNo2 = atoi(&(stationName2[1]));

    IF_INVALID_RETURN(stationNo1 < MAX_STATION_NUM, EN_RETURN_INVALID_STATION)
    IF_INVALID_RETURN(stationNo2 < MAX_STATION_NUM, EN_RETURN_INVALID_STATION)
    IF_INVALID_RETURN(g_stasionList[stationNo1].usrFlag, EN_RETURN_STATION_UNUSED)
    IF_INVALID_RETURN(g_stasionList[stationNo2].usrFlag, EN_RETURN_STATION_UNUSED)

    if(stationNo1 == stationNo2)
    {
        dis = 0;
        return EN_RETURN_SUCC;
    }

    dis = 0;
    if(0 == GetOneDerectDis(&(g_stasionList[stationNo1].list), EN_DERECT_NEXT, stationNo2, dis))
    {
        return EN_RETURN_SUCC;
    }

    dis = 0;
    if(0 == GetOneDerectDis(&(g_stasionList[stationNo1].list), EN_DERECT_PREV, stationNo2, dis))
    {
        return EN_RETURN_SUCC;
    }

    return EN_RETURN_INNER_ERR;
}

/*
@ 删除地铁线
@ 返回值: void
*/
void DelSubwayLine()
{
    InitSubwayLine();
    return;
}

/*
@ 打印所有地铁线
*/
void GetLineInfo(string &outStr)
{
    ostringstream oss;

    oss<<"地铁线路：";

    TListHead* pos = NULL;
    ST_STATION* tmpStation = NULL;
    ListForEach(pos, &(g_line.subwayLine))
    {
        tmpStation = CONTAINER_OF(pos, ST_STATION, list);
        oss<<"S"<<tmpStation->no;
        if(!ListIsLast(pos, &(g_line.subwayLine)))
        {
            oss<<"<->";
        }
    }
    outStr = oss.str();
}

/*
@ 设置默认地铁线路
@ 返回值: NULL
*/
void SetDefaultLine()
{
    AddSubwayStation("S0", 0);
    AddSubwayStation("S1", 800);
    AddSubwayStation("S2", 2168);
    AddSubwayStation("S3", 2098);
    AddSubwayStation("S4", 2165);
    AddSubwayStation("S5", 1500);
    AddSubwayStation("S6", 1345);
    AddSubwayStation("S7", 1567);
    AddSubwayStation("S8", 1897);
    AddSubwayStation("S9", 1000);
    AddSubwayStation("S10", 900);
    AddSubwayStation("S11", 1168);
    AddSubwayStation("S12", 2000);
    AddSubwayStation("S13", 1600);
    AddSubwayStation("S14", 1485);
    AddSubwayStation("S15", 1600);
    AddSubwayStation("S16", 1888);
}


#else

// -------------使用网状地铁线路

#define INFINITE_DISTANCE (MAX_STATION_NUM * MAX_DIS_BETWEEN_STATION + 1)

bool g_stationUsed[MAX_STATION_NUM];
unsigned int g_allDistance[MAX_STATION_NUM][MAX_STATION_NUM];  // 40 KB
unsigned int g_maxStationID;  // 38, this ID is also a used station.

/*
@ 获取两个地铁站间的最小距离
@ 返回值: EN_RETURN_SUCC: 成功, EN_RETURN_INVALID_LINE: 失败
*/
EN_RETURN_CODE GetSubwayStationDis(char* stationName1, char* stationName2, unsigned int &dis)
{
	unsigned int stationNo1 = atoi(&(stationName1[1]));
	unsigned int stationNo2 = atoi(&(stationName2[1]));

	IF_INVALID_RETURN(stationNo1 < MAX_STATION_NUM, EN_RETURN_INVALID_STATION)
	IF_INVALID_RETURN(stationNo2 < MAX_STATION_NUM, EN_RETURN_INVALID_STATION)
	IF_INVALID_RETURN(g_stationUsed[stationNo1], EN_RETURN_STATION_UNUSED)
	IF_INVALID_RETURN(g_stationUsed[stationNo2], EN_RETURN_STATION_UNUSED)

	dis = g_allDistance[stationNo1][stationNo2];
	return EN_RETURN_SUCC;
}


/*
@ 初始化地铁线路管理子系统
*/
void InitSubwayLine()
{
	for (int i = 0; i < MAX_STATION_NUM; ++i)
	{
		g_stationUsed[i] = false;
		for (int j = 0; j < MAX_STATION_NUM; ++j)
		{
			g_allDistance[i][j] = INFINITE_DISTANCE;
		}
	}
}


/*
@ 仅用于调试使用，查看距离矩阵内容。
*/
#include <fstream>

void PrintDistances(int rowN, int colN)
{
	ofstream outfile;
	outfile.open("DistanceOutput.txt", ios::app);
	if (!outfile) return;
	int row, col;
	for (row = 0; row <= rowN; ++row)
	{
		for (col = 0; col <= colN; ++col)
		{
			if (g_allDistance[row][col] < INFINITE_DISTANCE)
				outfile << g_allDistance[row][col] << "\t";
			else
				outfile << "****" << "\t";
		}
		outfile << endl;
	}
	outfile << endl;
	outfile.close();
}


/*
@ 设置默认地铁线路
@ 返回值: NULL
*/
void SetDefaultLine()
{
	int usedStationIndex = 0;
	for (usedStationIndex = 0; usedStationIndex <= 38; ++usedStationIndex)
	{
		g_stationUsed[usedStationIndex] = true;
		g_allDistance[usedStationIndex][usedStationIndex] = 0;
	}
		
	g_stationUsed[9] = false;
	g_stationUsed[13] = false; // S5 = S13
	g_stationUsed[19] = false;
	g_stationUsed[24] = false; // S15 = S24
	g_stationUsed[29] = false;
	g_stationUsed[33] = false; // S2 = S33
	g_stationUsed[34] = false; // S22 = S34

	g_allDistance[9][9] = INFINITE_DISTANCE;
	g_allDistance[13][13] = INFINITE_DISTANCE;
	g_allDistance[19][19] = INFINITE_DISTANCE;
	g_allDistance[24][24] = INFINITE_DISTANCE;
	g_allDistance[29][29] = INFINITE_DISTANCE;
	g_allDistance[33][33] = INFINITE_DISTANCE;
	g_allDistance[34][34] = INFINITE_DISTANCE;

	unsigned int line0[] = { 800, 2168, 2098, 2165, 1500, 1345, 1567, 1897 };
	unsigned int line1[] = { 900, 1168, 2198, 2000, 1600, 1485, 1600, 1888 };
	unsigned int line2[] = { 1100, 1238, 1998, 1800, 1700, 1585, 1405, 1822 };
	unsigned int line3[] = { 1110, 1338, 1568, 1450, 1680, 1345, 1555, 1682 };

	int smaller = 0;
	for (smaller = 0; smaller < 8; ++smaller)
		g_allDistance[smaller][smaller + 1] = line0[smaller];

	for (smaller = 10; smaller < 18; ++smaller)
		g_allDistance[smaller][smaller + 1] = line1[smaller - 10];
	g_allDistance[5][12] = g_allDistance[12][13];  // S5 = S13
	g_allDistance[12][13] = INFINITE_DISTANCE;
	g_allDistance[5][14] = g_allDistance[13][14];
	g_allDistance[13][14] = INFINITE_DISTANCE;

	for (smaller = 20; smaller < 28; ++smaller)
		g_allDistance[smaller][smaller + 1] = line2[smaller - 20];
	g_allDistance[15][23] = g_allDistance[23][24];  // S15 = S24
	g_allDistance[23][24] = INFINITE_DISTANCE;
	g_allDistance[15][25] = g_allDistance[24][25];
	g_allDistance[24][25] = INFINITE_DISTANCE;

	for (smaller = 30; smaller < 38; ++smaller)
		g_allDistance[smaller][smaller + 1] = line3[smaller - 30];
	g_allDistance[2][32] = g_allDistance[32][33];  // S2 = S33
	g_allDistance[32][33] = INFINITE_DISTANCE;
	g_allDistance[2][22] = g_allDistance[33][34];  // S22 = S34
	g_allDistance[33][34] = INFINITE_DISTANCE;
	g_allDistance[22][35] = g_allDistance[34][35];
	g_allDistance[34][35] = INFINITE_DISTANCE;

	unsigned int row = 0, col = 1;
	unsigned int sum = 0;
	for (col = 0; col < MAX_STATION_NUM; ++col)
	{
		for (row = col + 1; row < MAX_STATION_NUM; ++row)
		{
			g_allDistance[row][col] = g_allDistance[col][row];
		}
	}


	// ---------- Floyd algorithm for all-pairs-shortest-path
	for (size_t i = 0; i < MAX_STATION_NUM; i++)
	{
		if (g_stationUsed[i])
			g_maxStationID = i;
	}

	PrintDistances(g_maxStationID, g_maxStationID);

	unsigned int (*pMatrix[2])[MAX_STATION_NUM];
	int now = 1, last = 0, intSwitchTmp;
	unsigned int floydCache[MAX_STATION_NUM][MAX_STATION_NUM];
	pMatrix[0] = g_allDistance;
	pMatrix[1] = floydCache;

	// Copy from g_allDistance.
	for (row = 0; row <= g_maxStationID; ++row)
	{
		// if (!g_stationUsed[row]) continue;
		for (col = 0; col <= g_maxStationID; ++col)
		{
			// if (!g_stationUsed[col]) continue;
			pMatrix[1][row][col] = pMatrix[0][row][col];
		}
	}

	// Go n^3 iterations:
	for (size_t K = 0; K <= g_maxStationID; K++)
	{
		if (!g_stationUsed[K]) continue;
		for (row = 0; row <= g_maxStationID; row++)
		{
			if (!g_stationUsed[row]) continue;
			for (col = 0; col <= g_maxStationID; ++col)
			{
				if (!g_stationUsed[col]) continue;
				if (pMatrix[last][row][col] < (pMatrix[last][row][K] + pMatrix[last][K][col]))
				{
					pMatrix[now][row][col] = pMatrix[last][row][col];
				}
				else
				{
					pMatrix[now][row][col] = pMatrix[last][row][K] + pMatrix[last][K][col];
				}

			}
		}
		intSwitchTmp = now;
		now = last;
		last = intSwitchTmp;
	}

	// Copy the result to g_allDistance.
	if (now == 1) 
	{
		PrintDistances(g_maxStationID, g_maxStationID);
		return; 
	}
	for (row = 0; row <= g_maxStationID; ++row)
	{
		if (!g_stationUsed[row]) continue;
		for (col = 0; col <= g_maxStationID; ++col)
		{
			if (!g_stationUsed[col]) continue;
			g_allDistance[row][col] = floydCache[row][col];  // = pMatrix[1][row][col]
		}
	}

	PrintDistances(g_maxStationID, g_maxStationID);

}


/*
@ 打印所有地铁线
*/
void GetLineInfo(string &outStr)
{
	ostringstream oss;

	oss << "地铁线路0:S0<->S1<->S2<->S3<->S4<->S5<->S6<->S7<->S8\r\n";
	oss << "地铁线路1:S10<->S11<->S12<->S5<->S14<->S15<->S16<->S17<->S18\r\n";
	oss << "地铁线路2:S20<->S21<->S22<->S23<->S15<->S25<->S26<->S27<->S28\r\n";
	oss << "地铁线路3:S30<->S31<->S32<->S2<->S22<->S35<->S36<->S37<->S38";
	outStr = oss.str();
}

#endif
