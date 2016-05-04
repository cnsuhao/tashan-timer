

#include "include/stdafx.h"
#include "tsktimer/TskTimer.h"
#include "tsktimer/TskTimerMgr.h"
#include "newtask_frame.h"
#include "Fl_Drable_Window.h"
#include "utils/utils.h"

# include <stdio.h>
# include <stdlib.h>

#ifdef WIN32
#include <stdlib.h>	// atoi
#endif /*WIN32*/
#include <time.h>

void CNewTaskFrame::BuildTskEditWnd(IN CTskTimer* pTskTimer)
{
	if(!pTskTimer) return;
    m_pTskTimer = pTskTimer;

	CParams*   pTimeParam = pTskTimer->m_pTime;
    Fl_Button* pRdo=m_pCurrTimeRdo;
	TCHAR** vtDays=0;
    TCHAR** vtHours=0;

	if(pTimeParam)
	{
		switch(pTimeParam->chType)
		{
			//start date type：
			//00：当前(immediately)     01:阳历(solar calendar)     02:农历(Lunar calendar)
			//10：农历节日(solar calendar festival)  11：阳历节日(festival) 12：农历节气(feasts) 13:星期(week) 
			case 0:
				break;
			case 1:
				pRdo = m_psolarCalendarRdo;
				break;
		}
		pRdo->value(1);
		pRdo->setonly();
        pRdo->handle(FL_PUSH);
		pRdo->do_callback((Fl_Widget*)pRdo,(void*)0);

		time_t now;
		struct tm *tmNow;
		time(&now);
		tmNow   =   localtime(&now);

		int Y=0,M=0,D=0,y= tmNow->tm_year,m= tmNow->tm_mon,d=tmNow->tm_mday,h=tmNow->tm_hour,minu=tmNow->tm_min,sec=tmNow->tm_sec;
		//pTimeParam->m_pParam0 's format similar to  2013.2.24
	    int nCount =nsYLX::CUtil::Split(OUT vtDays, IN 3, IN  pTimeParam->m_pParam0, IN _T("."));

		if(nCount==3)
		{
			Y = _ttoi(vtDays[0]);
			M = _ttoi(vtDays[1]);
			D = _ttoi(vtDays[2]);

			m_yearChoice->value(Y-1900);
			m_monChoice->value(M);
			m_dayChoice->value(D);
		}
        nsYLX::CUtil::ClearMemForSplit(vtDays, nCount);
        vtDays=0;
        nCount =nsYLX::CUtil::Split(OUT vtDays, IN 3, IN  pTimeParam->m_pParam1, IN _T(":"));

		if(nCount==3)
        {
            m_hourIpt->value(vtDays[0]);
            m_minIpt->value(vtDays[1]);
            m_secIpt->value(vtDays[2]);
        }
        nsYLX::CUtil::ClearMemForSplit(vtDays, nCount);
	}

	CParams*   pOffsetParam  = pTskTimer->m_pOffset;
	if(pOffsetParam)
	{
		if(pOffsetParam->chType == 0)
			pRdo=m_beforeRdo;
		else
			pRdo=m_afterRdo;
		pRdo->value(1);
		pRdo->setonly();
        pRdo->handle(FL_PUSH);

        vtDays=0;
        vtHours=0;
        //pOffsetParam->m_pParam0 's format similar to  day.h:m:sec
	    int nCount =nsYLX::CUtil::Split(OUT vtDays, IN 2, IN  pOffsetParam->m_pParam0, IN _T("."));
		if(nCount==2)
        {
			m_offsetDIpt->value(vtDays[0]);
			int n = nsYLX::CUtil::Split(OUT vtHours, IN 3, IN  vtDays[1], IN _T(":"));
			nsYLX::CUtil::ClearMemForSplit(vtDays, nCount);
			if(n==3)
			{
				m_offsetHIpt->value(vtHours[0]);
				m_offsetMIpt->value(vtHours[1]);
				m_offsetSIpt->value(vtHours[2]);
			}
			nsYLX::CUtil::ClearMemForSplit(vtHours, n);
        }
	}//if(pOffsetParam
	CParams*   pFreqParam  = pTskTimer->m_pFreq;
    m_exectimesIpt->value("1");
	m_intvlMonIpt->value("0");
	m_intvlDIpt->value("0");
	m_intvlHIpt->value("0");
	m_intvlMinuIpt->value("0");
	m_intvlSIpt->value("0");
       
	if(pFreqParam)
	{
		if(pFreqParam->m_pParam0) m_exectimesIpt->value(pFreqParam->m_pParam0);
		if(pFreqParam->chType==0)
		{
			//months
			pRdo = m_freqMRdo;
		}else
		{
			//day,hour,...
			pRdo = m_freqDRdo;
		}
		pRdo->value(1);
		pRdo->setonly();
        

            
		if(pFreqParam->chType==0)
		{
			//pFreqParam->m_pParam1 's format similar to "5589"
			if(pFreqParam->m_pParam1) m_intvlMonIpt->value(pFreqParam->m_pParam1);
		}
		else
        {
			//pFreqParam->m_pParam1 's format similar to  day.h:m:sec
			int nCount =nsYLX::CUtil::Split(OUT vtDays, IN 2, IN  pFreqParam->m_pParam1, IN _T("."));
			if(nCount==2)
			{
				m_intvlDIpt->value(vtDays[0]);
				int n = nsYLX::CUtil::Split(OUT vtHours, IN 3, IN  vtDays[1], IN _T(":"));
				nsYLX::CUtil::ClearMemForSplit(vtDays, nCount);
				
				if(n==3)
				{
					m_intvlHIpt->value(vtHours[0]);
					m_intvlMinuIpt->value(vtHours[1]);
					m_intvlSIpt->value(vtHours[2]);
				}
				nsYLX::CUtil::ClearMemForSplit(vtHours, n);
			}
        }
	}//if(pFreqParam)
	if(pTskTimer->m_pTskName) m_pTskNameIpt->value(pTskTimer->m_pTskName);
    else m_pTskNameIpt->value("未命名");
    
	if(pTskTimer->m_pActions)
	{
		for(int i=0,n=pTskTimer->m_nActionsCount;i<n;i++)
		{
			CParams* pActionParam = pTskTimer->m_pActions[i];
			m_actionChoice->value(pActionParam->chType);
			//pActionParam->m_pParam0   exe's filename
			//pActionParam->m_pParam1   program's cmdline parameters
			//pActionParam->m_pParam2   exe's related filename
			m_exefnIpt->value(pActionParam->m_pParam0);
			m_execmdlineIpt->value(pActionParam->m_pParam1);
            m_actionChoice->do_callback((Fl_Widget*)m_actionChoice, (void*)0);

			break;//only dispaly the first task action
		}//for
	}//if(pTskTimer->m_nActionsCount>0)
}

