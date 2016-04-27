
#ifdef _WIN32
#include <windows.h>
#elif defined(__LINUX__)
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <inttypes.h>
#include <locale.h>
#include <fcntl.h>
#include <sys/types.h>
#endif // WIN32


#include "TskTimer.h"
#include "TskTimerMgr.h"

#include "../utils/utils.h"
//#include "../utils/calendar.h"


#if defined(WIN32) && defined(_DEBUG)
#define new DEBUG_NEW
#endif


bool CTskTimerMgr::CanDelTsk(IN CTskTimer* pTskTimerEntry)
{
	bool bCan = false;
	if (!pTskTimerEntry)
	{
		return true;
	}
	CListHead* plistEntry = &pTskTimerEntry->m_ListEntry;
	CListHead* pList = plistEntry->next;

	for(; pList != plistEntry ; )
	{
		CTskTimer* p = CONTAINING_RECORD(pList, CTskTimer, m_ListEntry);

		pList = pList->next;

		if (p->m_nState != FORDELETE)
		{
			break;
		}

		//在子任务链表里查
		bCan = CTskTimerMgr::CanDelTsk(p->m_pChildsEntry);
		if(!bCan)
		{
			break;	
		}
	}//for
	return bCan;
}//


LONGLONG CTskTimerMgr::GetTimeEspase(class CParams* pTime)
{
	LONGLONG   nElapse =0, llElapse=0;
 
	TCHAR** vtDays=0;
	TCHAR** vtHours=0;
#if 0
	CTimeSpan ts;
	CTime   tmNow( 0 ),ct(0) ;
	tmNow = CTime::GetCurrentTime();
	int Y=0,M=0,D=0,y= tmNow.GetYear(),m= tmNow.GetMonth(),d=tmNow.GetDay(),h=tmNow.GetHour(),minu=tmNow.GetMinute(),sec=tmNow.GetSecond();


	if (!pTime)
	{
		return 0;
	}

	//根据不同的闹钟类型，生成计时器的时间参数
	int yTmp,mTmp,dTmp,hTmp,minuTmp;
	switch(pTime->chType)
	{
	case 0:
		//即时闹钟
		nElapse=0;//立即执行
		break;
	case 1:
		//阳历日期闹钟
		//参数格式类似于  2013.2.24.13:05:11 ，这里我需要根据当前时间，算出 nElapse
		y=nsYLX::CUtil::Split(OUT vtDays, IN 3, IN  pTime->m_pParam0, IN _T("."));
		if (y!=3)
		{
			nsYLX::CUtil::ClearMemForSplit(vtDays,3);
			return -1;//格式不对呀
		}
		y = _ttoi(vtDays[0]);
		m = _ttoi(vtDays[1]);
		d = _ttoi(vtDays[2]);
		nsYLX::CUtil::ClearMemForSplit(vtDays,3);
		if (y != tmNow.GetYear() && m != tmNow.GetMonth() && d != tmNow.GetDay())
		{
			//闹钟不在今天，所以不做任何处理，呵呵
			return -1;
		}
		break;
	case 2:
		//阴历日期闹钟

		//参数格式类似于  2013.2.24.13:05:11 ，这里我需要根据当前时间，算出 nElapse
		Y=nsYLX::CUtil::Split(OUT vtDays, IN 3, IN  pTime->m_pParam0, IN _T("."));
		if (Y!=3)
		{
			nsYLX::CUtil::ClearMemForSplit(vtDays,3);
			return -1;//格式不对呀
		}
		Y = _ttoi(vtDays[0]);
		M = _ttoi(vtDays[1]);
		D = _ttoi(vtDays[2]);
		nsYLX::CUtil::ClearMemForSplit(vtDays,3);

		//首先将阴历转换成阳历
		if (!nsYLX::CCalendar::GetSolarDate( Y, M-1, D, y, m, d ))
		{
			break;
		} 

		if (y != tmNow.GetYear() && m != tmNow.GetMonth() && d != tmNow.GetDay())
		{
			//闹钟不在今天，所以不做任何处理，呵呵
			return -1;
		}
		break;
	case 10:
		{
			struct lunStru
			{
				int _mon;
				int _day;
			} _lunFestival[]=
			{
				{1,1}//"0101 春节",
				,{1,15}//"0115 元宵节",
				,{5,5}//"0505 端午节",
				,{7,7}//"0707 七夕情人节",
				,{7,15}//"0715 中元节",
				,{8,15}//"0815 中秋节",
				,{9,9}//"0909 重阳节",
				,{12,8}//"1208 腊八节",
				,{12,24}//"1224 小年",
				,{1,0}//"0100 除夕"
			};

			//农历节日闹钟 ,参数格式 x-x
			//首先将阴历转换成阳历
			Y=nsYLX::CUtil::Split(OUT vtDays, IN 2, IN  pTime->m_pParam0, IN _T("-"));
			if (Y!=2)
			{
				nsYLX::CUtil::ClearMemForSplit(vtDays,2);
				return -1;//格式不对呀
			}
			Y = _ttoi(vtDays[1]);
			nsYLX::CUtil::ClearMemForSplit(vtDays,2);

			if (!nsYLX::CCalendar::GetSolarDate( tmNow.GetYear(), _lunFestival[Y]._mon-1, _lunFestival[Y]._day, yTmp,mTmp,dTmp ))
			{
				break;
			}

			if (mTmp != tmNow.GetMonth() && dTmp != tmNow.GetDay())
			{
				//今天不过节，所以不做任何处理，呵呵
				return -1;
			}
		}

		break;
	case 11:
		{
			//阳历节日闹钟
			struct lunStru
			{
				int _mon;
				int _day;
			} _SolarFestival[]=
			{ 
				{1,1}//元旦,
				,{2,14}//情人节
				,{3,3}//爱耳日
				,{3,5}//青年志愿者服务日
				,{3,7}//女生节
				,{3,8}//妇女节
				,{3,12}//植树节
				,{3,14}//白色情人节,国际警察日
				,{3,15}//消费者权益日
				,{3,21}//世界森林日,睡眠日
				,{3,22}//世界水日
				,{3,23}//世界气象日
				,{3,24}//世界防治结核病日
				,{4,1}//愚人节
				,{4,4}//清明节
				,{4,7}//世界卫生日
				,{4,22}//世界地球日
				,{4,26}//世界知识产权日
				,{5,1}//劳动节
				,{5,3}//世界哮喘日
				,{5,4}//青年节
				,{5,8}//世界红十字日,世界微笑日
				,{5,12}//护士节
				,{5,15}//国际家庭日
				,{5,31}//世界无烟日
				,{6,1}//儿童节
				,{6,5}//世界环境日
				,{6,6}//全国爱眼日
				,{6,11}//中国人口日
				,{6,17}//世界防止荒漠化和干旱日
				,{6,20}//世界难民日
				,{6,23}//国际奥林匹克日
				,{6,25}//全国土地日
				,{6,26}//国际禁毒日
				,{7,1}//建党节 香港回归纪念
				,{7,7}//抗日战争纪念日
				,{7,11}//世界人口日
				,{8,1}//建军节
				,{8,6}//国际电影节  
				,{8,8}//中国男子节
				,{8,12}//国际青年节
				,{8,15}//日本投降日
				,{9,3}//中国抗日战争胜利纪念日
				,{9,8}//国际扫盲日
				,{9,9}//毛席逝世纪念
				,{9,10}//教师节
				,{9,16}//中国脑健康日,国际臭氧层保护日
				,{9,18}//九·一八事变纪念日,中国国耻日
				,{9,20}//全国爱牙日
				,{9,21}//世界停火日
				,{9,27}//世界旅游日
				,{9,28}//孔子诞辰
				,{10,1}//国庆节
				,{10,6}//老人节
				,{10,16}//世界粮食日
				,{10,17}//国际消除贫困日
				,{10,24}//联合国日
				,{10,31}//万圣节
				,{11,8}//中国记者节
				,{11,12}//孙中山诞辰
				,{12,1}//艾滋病日 
				,{12,3}//世界残疾人日
				,{12,13}//南京大屠杀悼念日
				,{12,20}//澳门回归纪念
				,{12,25}//圣诞节
				,{12,26}//毛席诞辰 
			};

			Y=nsYLX::CUtil::Split(OUT vtDays, IN 2, IN  pTime->m_pParam0, IN _T("-"));
			if (Y!=2)
			{
				nsYLX::CUtil::ClearMemForSplit(vtDays,2);
				return -1;//格式不对呀
			}
			Y = _ttoi(vtDays[1]);
			nsYLX::CUtil::ClearMemForSplit(vtDays,2);

			yTmp = tmNow.GetYear();
			mTmp = _SolarFestival[Y]._mon;
			dTmp = _SolarFestival[Y]._day;
			if (mTmp != tmNow.GetMonth() && dTmp != tmNow.GetDay())
			{
				//今天不是节日，所以不做任何处理，呵呵
				return -1;
			}
		}
		break;
	case 12:
		//农历节气闹钟
		Y=nsYLX::CUtil::Split(OUT vtDays, IN 2, IN  pTime->m_pParam0, IN _T("-"));
		if (Y!=2)
		{
			nsYLX::CUtil::ClearMemForSplit(vtDays,2);
			return -1;//格式不对呀
		}
		Y = _ttoi(vtDays[1]);
		nsYLX::CUtil::ClearMemForSplit(vtDays,2);
		sec =0;
		nsYLX::CCalendar::GetTermTime( y, Y,yTmp,mTmp,dTmp, hTmp,minuTmp );

		if (mTmp != tmNow.GetMonth() &&dTmp != tmNow.GetDay())
		{
			//今天不是节气，所以不做任何处理，呵呵
			return -1;
		}
		break;
	case 13:
		//星期闹钟
		{
			Y=nsYLX::CUtil::Split(OUT vtDays, IN 2, IN  pTime->m_pParam0, IN _T("-"));
			if (Y!=2)
			{
				nsYLX::CUtil::ClearMemForSplit(vtDays,2);
				return -1;//格式不对呀
			} 
			h=nsYLX::CUtil::Split(OUT vtHours, IN 7, IN  vtDays[1], IN _T(","));
			nsYLX::CUtil::ClearMemForSplit(vtDays,2);

			//ct = CTime( 2013, 2,24,1,1,1);
			int nTodayWeek = tmNow.GetDayOfWeek() - 1;
			int i=0;
			for (  i=0;i<h;i++)
			{
				int nWeekDay = _ttoi(vtHours[i]);//周几
				if (nTodayWeek == nWeekDay)
				{
					//可能有很多星期，但我只处理属于今天的定时器任务。
					break;
				} 
			}

			if (i == h)
			{
				// 没找到当天的计时任务，
				nsYLX::CUtil::ClearMemForSplit(vtHours,h);
				return -1;
			}

			nsYLX::CUtil::ClearMemForSplit(vtHours,h);
		}
		break;
	}

	if (pTime->chType != 0)
	{
		h=nsYLX::CUtil::Split(OUT vtHours, IN 3, IN  pTime->m_pParam1, IN _T(":"));
		if (h!=3)
		{
			nsYLX::CUtil::ClearMemForSplit(vtHours,3);
			return -1;//格式不对呀
		}

		h = _ttoi(vtHours[0]);
		minu = _ttoi(vtHours[1]);
		sec = _ttoi(vtHours[2]);

		nsYLX::CUtil::ClearMemForSplit(vtHours,3);	

		//ATLENSURE( nYear >= 1900 );
		//ATLENSURE( nMonth >= 1 && nMonth <= 12 );
		//ATLENSURE( nDay >= 1 && nDay <= 31 );
		//ATLENSURE( nHour >= 0 && nHour <= 23 );
		//ATLENSURE( nMin >= 0 && nMin <= 59 );
		//ATLENSURE( nSec >= 0 && nSec <= 59 );
		//ct = CTime( y,m,d,h,minu,sec);

		struct tm atm;
		atm.tm_sec = sec;
		atm.tm_min = minu;
		atm.tm_hour = h;
		atm.tm_mday = d;
		atm.tm_mon = m - 1;        // tm_mon is 0 based
		atm.tm_year = y - 1900;     // tm_year is 1900 based
		atm.tm_isdst = -1;

		__time64_t _time = _mktime64(&atm);
		ct = _time;

		ts= ct-tmNow;
		llElapse = ts.GetTotalSeconds();
		if (llElapse<0)
		{
			//过期了的定时器，本应不做处理，但考虑到周期循环，不能简单废弃
			//if (nFreqTimes > 0)
			{

			}
		}
	}//

	nElapse = llElapse;//*1000;
    #endif
	return nElapse;
} 


LONGLONG CTskTimerMgr::GetOffsetEspase(class CParams* pOffset)
{
	if (!pOffset)
	{
		return 0;
	}
#if 0
	CTimeSpan ts;

	int y,m,d,h,minu,sec;

	TCHAR** vtDays=0;
	TCHAR** vtHours=0;

	y=nsYLX::CUtil::Split(OUT vtDays, IN 2, IN  pOffset->m_pParam0, IN _T("."));

	do 
	{
		if (y!=2)
		{
			nsYLX::CUtil::ClearMemForSplit(vtDays,2);
			ATLASSERT(0);
			break;//格式不对呀
		}

		d = _ttoi(vtDays[0]);
		h=nsYLX::CUtil::Split(OUT vtHours, IN 3, IN  vtDays[1], IN _T(":"));
		if (h!=3)
		{
			nsYLX::CUtil::ClearMemForSplit(vtDays,2);
			nsYLX::CUtil::ClearMemForSplit(vtHours,3);
			ATLASSERT(0);
			break;//格式不对呀
		}
		h = _ttoi(vtHours[0]);
		minu = _ttoi(vtHours[1]);
		sec = _ttoi(vtHours[2]);

		nsYLX::CUtil::ClearMemForSplit(vtDays,2);
	} while (0);

	nsYLX::CUtil::ClearMemForSplit(vtHours,3);

	ts=CTimeSpan(d,h,minu,sec);//= ct-tmNow;

	return  ts.GetTotalSeconds();// (tmNow-ts).GetTime/*GetTotalSeconds*/();
#endif
    return 0;
}//LONGLONG CTskTimerMgr::GetOffsetEspase(class CParams* pOffset)


int CTskTimerMgr::GetDaysOfCurrMonth(int nMonth, int nYear)
{
	bool bIsLeap = (nYear%400==0) || (nYear%100!=0&& nYear%4==0);

	int nTotalDays = 0;
	switch (nMonth)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		nTotalDays=31;
		break;

	case 2:
		if (bIsLeap)
		{
			nTotalDays = 29;
		}else
		{
			nTotalDays=28;
		}
		break;

	default:
		nTotalDays=30;
		break;
	}
	return nTotalDays;
}//int CTskTimerMgr::GetDaysOfCurrMonth(int nMonth, int nYear)


bool CTskTimerMgr::GetDateNxtSecs(int* pY, int* pMon, int* pD, int* pH, int* pMin, int* pS, int nOffset)
{
	*pS    += nOffset;
	//表示秒钟，是从 0 到 59 的整数
	if(*pS < 0)
	{
		*pMin   += (*pS)/60;
		*pS      = 60+(*pS)%60;
		return CTskTimerMgr::GetDateNxtSecs(pY,        pMon,       pD,                                    pH,             pMin,   pS,    0);
	}else if (*pS > 59 )
	{
		*pMin = (*pMin)+( (*pS)/60 );
		*pS   = (*pS) % 60;
		nOffset = 0;
		return CTskTimerMgr::GetDateNxtSecs(pY,        pMon,       pD,                                    pH,             pMin,   pS,    nOffset);
	}

	//表示分钟，是从 0 到 59 的整数
	if (*pMin < 0)
	{
		*pH   += (*pMin)/60;
		*pMin  = 60+(*pMin)%60;
		return CTskTimerMgr::GetDateNxtSecs(pY,        pMon,       pD,                                    pH,             pMin,   pS,    nOffset);
	}else if (*pMin >59)
	{ 
		*pH    = (*pH) + ((*pMin)/60);
		*pMin  = (*pMin) % 60;
		return CTskTimerMgr::GetDateNxtSecs(pY,        pMon,       pD,                                    pH,             pMin,   pS,    nOffset);
	}

	//表示小时，是从 0 到 23 的整数
	if(*pH < 0)
	{
		*pD   += (*pH)/24;
		*pH    = 24+(*pH)%24;
		return CTskTimerMgr::GetDateNxtSecs(pY,        pMon,       pD,                                    pH,             pMin,   pS,    nOffset);
	}else if(*pH > 23)
	{
		*pD  = (*pD) + ((*pH)/24);
		*pH  = (*pH) % 24;
		return CTskTimerMgr::GetDateNxtSecs(pY,        pMon,       pD,                                    pH,             pMin,   pS,    nOffset);
	}

	//表示日期，是从 1 到 31 之间的整数
	if(*pD < 0)
	{
		*pMon=(*pMon)--;
		*pD  = CTskTimerMgr::GetDaysOfCurrMonth(*pMon,*pY)+*pD;
		return CTskTimerMgr::GetDateNxtSecs(pY,        pMon,       pD,                                    pH,             pMin,   pS,    nOffset);
	}else if(*pD > CTskTimerMgr::GetDaysOfCurrMonth(*pMon,*pY))
	{
		*pD   -= CTskTimerMgr::GetDaysOfCurrMonth(*pMon,*pY);
		*pMon  = (*pMon) + 1;
		return CTskTimerMgr::GetDateNxtSecs(pY,        pMon,       pD,                                    pH,             pMin,   pS,    nOffset);
	}

	//表示的月份，是从 0 到 11 之间的整数
	if(*pMon < 0)
	{
		*pY=(*pY)--;
		*pMon  = 12+(*pMon);
		return CTskTimerMgr::GetDateNxtSecs(pY,        pMon,       pD,                                    pH,             pMin,   pS,    nOffset);
	}else if(*pMon > 12)
	{
		*pMon -= 12;
		*pY  = (*pY) + 1;
		return CTskTimerMgr::GetDateNxtSecs(pY,        pMon,       pD,                                    pH,             pMin,   pS,    nOffset);
	}

	if (*pY<1900)
	{
		*pY = 1900;
	}  
	return true;
}

//d<0表示获取指定日期的前d天的日期,m以1为下标，最大值12表示第12个月,d同样以1为下标，最大值31表示该月的第31天
bool CTskTimerMgr::GetDateNxtDays(int y,int m,int d,int& newY,int& newM,int& newD,int nOffsetDays)
{
	newY = y;
	newM = m;
	newD = d+nOffsetDays;

	if (newD<1)
	{
		//此时nOffsetDays一定小于0，表示希望获得前nOffsetDays天日期
		return CTskTimerMgr::GetDateNxtDays(  y,  m-1,  GetDaysOfCurrMonth(m-1,y),  newY,  newM,  newD,  nOffsetDays-d);
	} 

	if (m<1)
	{
		return CTskTimerMgr::GetDateNxtDays(  y-1,  12,  GetDaysOfCurrMonth(12,y-1),  newY,  newM,  newD,  nOffsetDays-d);
	}

	int nDaysCurMonth = GetDaysOfCurrMonth(m,y);
	if (newD > nDaysCurMonth)
	{
		//此时nOffsetDays一定大于0，表示希望获得后nOffsetDays天日期
		return CTskTimerMgr::GetDateNxtDays(  y,  m+1,  0,  newY,  newM,  newD,  newD-nDaysCurMonth);
	}

	if (m>12)
	{
		return CTskTimerMgr::GetDateNxtDays(  y+1,  1,  0,  newY,  newM,  newD, newD);
	}

	return true;
}//

// llTotalSecsOffsetNow 距离现在的总秒数
LONGLONG CTskTimerMgr::GetFreqEspase(CParams* pFreq, const LONGLONG llTotalSecsOffsetNow)
{
	if (!pFreq)
	{
		return 0;
	}

	int nFreqTimes = 0;//_ttoi(pFreq->m_pParam0);
	LONGLONG   nElapse =0, llElapse=0;     
#if 0
	CTimeSpan ts(llTotalSecsOffsetNow);
	CTime   tmNow( 0 ),ct(0) ;
	tmNow = CTime::GetCurrentTime() + ts ;
	int Y=0,M=0,D=0,y= tmNow.GetYear(),m= tmNow.GetMonth(),d=tmNow.GetDay(),h=tmNow.GetHour(),minu=tmNow.GetMinute(),sec=tmNow.GetSecond();
 


	switch(pFreq->chType)
	{
	case 0://x月
		{
			llElapse =_ttoi(pFreq->m_pParam1);
			int nTotalDays = 0;
			for (int iMon =0;iMon<llElapse;iMon++)
			{
				nTotalDays += nsYLX::CCalendar::GetSolarMonthDays( tmNow.GetYear(), (tmNow.GetMonth()+2+iMon)%12);
			}
			llElapse  = nTotalDays * 24 * 3600 ;//换算成 秒
		}
		break;
	case 1:
		//x日.x时:x分:x秒
		{ 
			int yTmp=0,mTmp=0,dTmp=0,hTmp=0,minuTmp=0,secTmp=0;

			TCHAR** vtDays=0;
			TCHAR** vtHours=0;

			yTmp=nsYLX::CUtil::Split(OUT vtDays, IN 2, IN  pFreq->m_pParam1, IN _T("."));
			if (yTmp!=2)
			{
				nsYLX::CUtil::ClearMemForSplit(vtDays,2);
				return false;//格式不对呀
			}

			dTmp = _ttoi(vtDays[0]);
			hTmp = nsYLX::CUtil::Split(OUT vtHours, IN 3, IN  vtDays[1], IN _T(":"));
			if (hTmp != 3)
			{
				nsYLX::CUtil::ClearMemForSplit(vtDays,2);
				nsYLX::CUtil::ClearMemForSplit(vtHours,3);
				return false;//格式不对呀
			}
			hTmp    = _ttoi(vtHours[0]);
			minuTmp = _ttoi(vtHours[1]);
			secTmp  = _ttoi(vtHours[2]);

			nsYLX::CUtil::ClearMemForSplit(vtDays,2);
			nsYLX::CUtil::ClearMemForSplit(vtHours,3);

			llElapse  = ((dTmp * 24 + hTmp)*60 + minuTmp) * 60 +secTmp;//换算成 秒

		}
		break;

	}//switch
#endif
	return llElapse;
}//

 
