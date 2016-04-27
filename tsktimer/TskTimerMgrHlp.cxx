
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

		//���������������
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

	//���ݲ�ͬ���������ͣ����ɼ�ʱ����ʱ�����
	int yTmp,mTmp,dTmp,hTmp,minuTmp;
	switch(pTime->chType)
	{
	case 0:
		//��ʱ����
		nElapse=0;//����ִ��
		break;
	case 1:
		//������������
		//������ʽ������  2013.2.24.13:05:11 ����������Ҫ���ݵ�ǰʱ�䣬��� nElapse
		y=nsYLX::CUtil::Split(OUT vtDays, IN 3, IN  pTime->m_pParam0, IN _T("."));
		if (y!=3)
		{
			nsYLX::CUtil::ClearMemForSplit(vtDays,3);
			return -1;//��ʽ����ѽ
		}
		y = _ttoi(vtDays[0]);
		m = _ttoi(vtDays[1]);
		d = _ttoi(vtDays[2]);
		nsYLX::CUtil::ClearMemForSplit(vtDays,3);
		if (y != tmNow.GetYear() && m != tmNow.GetMonth() && d != tmNow.GetDay())
		{
			//���Ӳ��ڽ��죬���Բ����κδ����Ǻ�
			return -1;
		}
		break;
	case 2:
		//������������

		//������ʽ������  2013.2.24.13:05:11 ����������Ҫ���ݵ�ǰʱ�䣬��� nElapse
		Y=nsYLX::CUtil::Split(OUT vtDays, IN 3, IN  pTime->m_pParam0, IN _T("."));
		if (Y!=3)
		{
			nsYLX::CUtil::ClearMemForSplit(vtDays,3);
			return -1;//��ʽ����ѽ
		}
		Y = _ttoi(vtDays[0]);
		M = _ttoi(vtDays[1]);
		D = _ttoi(vtDays[2]);
		nsYLX::CUtil::ClearMemForSplit(vtDays,3);

		//���Ƚ�����ת��������
		if (!nsYLX::CCalendar::GetSolarDate( Y, M-1, D, y, m, d ))
		{
			break;
		} 

		if (y != tmNow.GetYear() && m != tmNow.GetMonth() && d != tmNow.GetDay())
		{
			//���Ӳ��ڽ��죬���Բ����κδ����Ǻ�
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
				{1,1}//"0101 ����",
				,{1,15}//"0115 Ԫ����",
				,{5,5}//"0505 �����",
				,{7,7}//"0707 ��Ϧ���˽�",
				,{7,15}//"0715 ��Ԫ��",
				,{8,15}//"0815 �����",
				,{9,9}//"0909 ������",
				,{12,8}//"1208 ���˽�",
				,{12,24}//"1224 С��",
				,{1,0}//"0100 ��Ϧ"
			};

			//ũ���������� ,������ʽ x-x
			//���Ƚ�����ת��������
			Y=nsYLX::CUtil::Split(OUT vtDays, IN 2, IN  pTime->m_pParam0, IN _T("-"));
			if (Y!=2)
			{
				nsYLX::CUtil::ClearMemForSplit(vtDays,2);
				return -1;//��ʽ����ѽ
			}
			Y = _ttoi(vtDays[1]);
			nsYLX::CUtil::ClearMemForSplit(vtDays,2);

			if (!nsYLX::CCalendar::GetSolarDate( tmNow.GetYear(), _lunFestival[Y]._mon-1, _lunFestival[Y]._day, yTmp,mTmp,dTmp ))
			{
				break;
			}

			if (mTmp != tmNow.GetMonth() && dTmp != tmNow.GetDay())
			{
				//���첻���ڣ����Բ����κδ����Ǻ�
				return -1;
			}
		}

		break;
	case 11:
		{
			//������������
			struct lunStru
			{
				int _mon;
				int _day;
			} _SolarFestival[]=
			{ 
				{1,1}//Ԫ��,
				,{2,14}//���˽�
				,{3,3}//������
				,{3,5}//����־Ը�߷�����
				,{3,7}//Ů����
				,{3,8}//��Ů��
				,{3,12}//ֲ����
				,{3,14}//��ɫ���˽�,���ʾ�����
				,{3,15}//������Ȩ����
				,{3,21}//����ɭ����,˯����
				,{3,22}//����ˮ��
				,{3,23}//����������
				,{3,24}//������ν�˲���
				,{4,1}//���˽�
				,{4,4}//������
				,{4,7}//����������
				,{4,22}//���������
				,{4,26}//����֪ʶ��Ȩ��
				,{5,1}//�Ͷ���
				,{5,3}//����������
				,{5,4}//�����
				,{5,8}//�����ʮ����,����΢Ц��
				,{5,12}//��ʿ��
				,{5,15}//���ʼ�ͥ��
				,{5,31}//����������
				,{6,1}//��ͯ��
				,{6,5}//���绷����
				,{6,6}//ȫ��������
				,{6,11}//�й��˿���
				,{6,17}//�����ֹ��Į���͸ɺ���
				,{6,20}//����������
				,{6,23}//���ʰ���ƥ����
				,{6,25}//ȫ��������
				,{6,26}//���ʽ�����
				,{7,1}//������ ��ۻع����
				,{7,7}//����ս��������
				,{7,11}//�����˿���
				,{8,1}//������
				,{8,6}//���ʵ�Ӱ��  
				,{8,8}//�й����ӽ�
				,{8,12}//���������
				,{8,15}//�ձ�Ͷ����
				,{9,3}//�й�����ս��ʤ��������
				,{9,8}//����ɨä��
				,{9,9}//ëϯ��������
				,{9,10}//��ʦ��
				,{9,16}//�й��Խ�����,���ʳ����㱣����
				,{9,18}//�š�һ���±������,�й�������
				,{9,20}//ȫ��������
				,{9,21}//����ͣ����
				,{9,27}//����������
				,{9,28}//���ӵ���
				,{10,1}//�����
				,{10,6}//���˽�
				,{10,16}//������ʳ��
				,{10,17}//��������ƶ����
				,{10,24}//���Ϲ���
				,{10,31}//��ʥ��
				,{11,8}//�й����߽�
				,{11,12}//����ɽ����
				,{12,1}//���̲��� 
				,{12,3}//����м�����
				,{12,13}//�Ͼ�����ɱ������
				,{12,20}//���Żع����
				,{12,25}//ʥ����
				,{12,26}//ëϯ���� 
			};

			Y=nsYLX::CUtil::Split(OUT vtDays, IN 2, IN  pTime->m_pParam0, IN _T("-"));
			if (Y!=2)
			{
				nsYLX::CUtil::ClearMemForSplit(vtDays,2);
				return -1;//��ʽ����ѽ
			}
			Y = _ttoi(vtDays[1]);
			nsYLX::CUtil::ClearMemForSplit(vtDays,2);

			yTmp = tmNow.GetYear();
			mTmp = _SolarFestival[Y]._mon;
			dTmp = _SolarFestival[Y]._day;
			if (mTmp != tmNow.GetMonth() && dTmp != tmNow.GetDay())
			{
				//���첻�ǽ��գ����Բ����κδ����Ǻ�
				return -1;
			}
		}
		break;
	case 12:
		//ũ����������
		Y=nsYLX::CUtil::Split(OUT vtDays, IN 2, IN  pTime->m_pParam0, IN _T("-"));
		if (Y!=2)
		{
			nsYLX::CUtil::ClearMemForSplit(vtDays,2);
			return -1;//��ʽ����ѽ
		}
		Y = _ttoi(vtDays[1]);
		nsYLX::CUtil::ClearMemForSplit(vtDays,2);
		sec =0;
		nsYLX::CCalendar::GetTermTime( y, Y,yTmp,mTmp,dTmp, hTmp,minuTmp );

		if (mTmp != tmNow.GetMonth() &&dTmp != tmNow.GetDay())
		{
			//���첻�ǽ��������Բ����κδ����Ǻ�
			return -1;
		}
		break;
	case 13:
		//��������
		{
			Y=nsYLX::CUtil::Split(OUT vtDays, IN 2, IN  pTime->m_pParam0, IN _T("-"));
			if (Y!=2)
			{
				nsYLX::CUtil::ClearMemForSplit(vtDays,2);
				return -1;//��ʽ����ѽ
			} 
			h=nsYLX::CUtil::Split(OUT vtHours, IN 7, IN  vtDays[1], IN _T(","));
			nsYLX::CUtil::ClearMemForSplit(vtDays,2);

			//ct = CTime( 2013, 2,24,1,1,1);
			int nTodayWeek = tmNow.GetDayOfWeek() - 1;
			int i=0;
			for (  i=0;i<h;i++)
			{
				int nWeekDay = _ttoi(vtHours[i]);//�ܼ�
				if (nTodayWeek == nWeekDay)
				{
					//�����кܶ����ڣ�����ֻ�������ڽ���Ķ�ʱ������
					break;
				} 
			}

			if (i == h)
			{
				// û�ҵ�����ļ�ʱ����
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
			return -1;//��ʽ����ѽ
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
			//�����˵Ķ�ʱ������Ӧ�������������ǵ�����ѭ�������ܼ򵥷���
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
			break;//��ʽ����ѽ
		}

		d = _ttoi(vtDays[0]);
		h=nsYLX::CUtil::Split(OUT vtHours, IN 3, IN  vtDays[1], IN _T(":"));
		if (h!=3)
		{
			nsYLX::CUtil::ClearMemForSplit(vtDays,2);
			nsYLX::CUtil::ClearMemForSplit(vtHours,3);
			ATLASSERT(0);
			break;//��ʽ����ѽ
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
	//��ʾ���ӣ��Ǵ� 0 �� 59 ������
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

	//��ʾ���ӣ��Ǵ� 0 �� 59 ������
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

	//��ʾСʱ���Ǵ� 0 �� 23 ������
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

	//��ʾ���ڣ��Ǵ� 1 �� 31 ֮�������
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

	//��ʾ���·ݣ��Ǵ� 0 �� 11 ֮�������
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

//d<0��ʾ��ȡָ�����ڵ�ǰd�������,m��1Ϊ�±꣬���ֵ12��ʾ��12����,dͬ����1Ϊ�±꣬���ֵ31��ʾ���µĵ�31��
bool CTskTimerMgr::GetDateNxtDays(int y,int m,int d,int& newY,int& newM,int& newD,int nOffsetDays)
{
	newY = y;
	newM = m;
	newD = d+nOffsetDays;

	if (newD<1)
	{
		//��ʱnOffsetDaysһ��С��0����ʾϣ�����ǰnOffsetDays������
		return CTskTimerMgr::GetDateNxtDays(  y,  m-1,  GetDaysOfCurrMonth(m-1,y),  newY,  newM,  newD,  nOffsetDays-d);
	} 

	if (m<1)
	{
		return CTskTimerMgr::GetDateNxtDays(  y-1,  12,  GetDaysOfCurrMonth(12,y-1),  newY,  newM,  newD,  nOffsetDays-d);
	}

	int nDaysCurMonth = GetDaysOfCurrMonth(m,y);
	if (newD > nDaysCurMonth)
	{
		//��ʱnOffsetDaysһ������0����ʾϣ����ú�nOffsetDays������
		return CTskTimerMgr::GetDateNxtDays(  y,  m+1,  0,  newY,  newM,  newD,  newD-nDaysCurMonth);
	}

	if (m>12)
	{
		return CTskTimerMgr::GetDateNxtDays(  y+1,  1,  0,  newY,  newM,  newD, newD);
	}

	return true;
}//

// llTotalSecsOffsetNow �������ڵ�������
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
	case 0://x��
		{
			llElapse =_ttoi(pFreq->m_pParam1);
			int nTotalDays = 0;
			for (int iMon =0;iMon<llElapse;iMon++)
			{
				nTotalDays += nsYLX::CCalendar::GetSolarMonthDays( tmNow.GetYear(), (tmNow.GetMonth()+2+iMon)%12);
			}
			llElapse  = nTotalDays * 24 * 3600 ;//����� ��
		}
		break;
	case 1:
		//x��.xʱ:x��:x��
		{ 
			int yTmp=0,mTmp=0,dTmp=0,hTmp=0,minuTmp=0,secTmp=0;

			TCHAR** vtDays=0;
			TCHAR** vtHours=0;

			yTmp=nsYLX::CUtil::Split(OUT vtDays, IN 2, IN  pFreq->m_pParam1, IN _T("."));
			if (yTmp!=2)
			{
				nsYLX::CUtil::ClearMemForSplit(vtDays,2);
				return false;//��ʽ����ѽ
			}

			dTmp = _ttoi(vtDays[0]);
			hTmp = nsYLX::CUtil::Split(OUT vtHours, IN 3, IN  vtDays[1], IN _T(":"));
			if (hTmp != 3)
			{
				nsYLX::CUtil::ClearMemForSplit(vtDays,2);
				nsYLX::CUtil::ClearMemForSplit(vtHours,3);
				return false;//��ʽ����ѽ
			}
			hTmp    = _ttoi(vtHours[0]);
			minuTmp = _ttoi(vtHours[1]);
			secTmp  = _ttoi(vtHours[2]);

			nsYLX::CUtil::ClearMemForSplit(vtDays,2);
			nsYLX::CUtil::ClearMemForSplit(vtHours,3);

			llElapse  = ((dTmp * 24 + hTmp)*60 + minuTmp) * 60 +secTmp;//����� ��

		}
		break;

	}//switch
#endif
	return llElapse;
}//

 
