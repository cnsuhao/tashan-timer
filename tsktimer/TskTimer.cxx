
#include "../include/stdafx.h"
#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#else
#include <stdio.h>
#include "../include/auto_tchar.h"
#endif

#include "TskTimer.h"
#include "../utils/utils.h"


#if defined(WIN32) && defined(_DEBUG)
#define new DEBUG_NEW
#endif

CParams::CParams()
{
	chType =0x0; 
	m_nId = 0;//只对动作变量有效

	m_pParam0 = 0;
	m_pParam1 = 0;
	m_pParam2 = 0;
}

CParams::~CParams()
{ 
	if (m_pParam0)
	{
	    free(m_pParam0);m_pParam0=0;
	}

	if (m_pParam1)
	{
	    free(m_pParam1);m_pParam1=0;
	}

	if (m_pParam2)
	{
	    free(m_pParam2);m_pParam2=0;
	}
}
	
CParams::CParams(const CParams& other)
{
   *this = other;
}

void CParams::CopyParam(TCHAR*& pDes,const TCHAR* pSrc)
{
	if (pSrc)
	{
		int nLen = _tcslen(pSrc)+1;

		if (!pDes) 
		{
			pDes = (TCHAR*)malloc(MAX_PATH*sizeof(TCHAR));
		}else 
			pDes = (TCHAR*)realloc(pDes, nLen*sizeof(TCHAR));
		pDes[0]=0;
		_tcscpy(pDes, pSrc);		
	}
}

CParams& CParams::operator=(const CParams& other)
{
	if (this != &other)
	{
		chType    = other.chType;

		CParams::CopyParam(m_pParam0, other.m_pParam0);
		CParams::CopyParam(m_pParam1, other.m_pParam1);
		CParams::CopyParam(m_pParam2, other.m_pParam2);
	}
	return *this;
}


//////////////////////////////////////////////////////////////////
void CTskTimer::ValidTime(int& val, ETIMETYPE e)
{
    if(IsValidTime(val,e)) return;
    switch(e)
    {
    case ETT_YEAR:
        val = 2016;
        break;
    case ETT_MONTH:
        val = 6;
        break;
    case ETT_DAY:
        val = 15;
        break;
    case ETT_HOUR:
        val=7;
        break;
    case ETT_MINU:
    case ETT_SEC:
        val=30;
        break;
    }
}


bool CTskTimer::IsValidTime(int val, ETIMETYPE e)
{
    switch(e)
    {
    case ETT_YEAR:
        return IsValidNum(val,1900,2050);
        break;
    case ETT_MONTH:
        return IsValidNum(val,0,11);
        break;
    case ETT_DAY:
        return IsValidNum(val,1,31);
        break;
    case ETT_HOUR:
        return IsValidNum(val,0,23);
        break;
    case ETT_MINU:
        return IsValidNum(val,0,59);
        break;
    case ETT_SEC:
        return IsValidNum(val,0,60);//1 leap second
        break;
    }
    return false;
}

bool CTskTimer::IsValidNum(int v, int minV, int maxV)
{
    return (v<=maxV && v>=minV);
}
////////////////////////////////////////////////////////////////
CTskTimer::CTskTimer(void)
{
	m_pTskName = 0;
	m_pTime = 0;
	m_pNextTrigTime=0;
	m_pOffset = new CParams;
	m_pFreq = new CParams;
	m_pActions = 0;
	m_nActionsCount=0;

	m_pChildsEntry = 0;//需要一个空头结点
	
	m_nId = -1;
	m_nState = 0;
	m_llTotalElapse=0;
#ifdef WIN32
	m_hPauseEvent  //= CreateEvent( NULL, TRUE, TRUE, NULL );
	= CreateSemaphore( 
		NULL,   // default security attributes
		0,		// initial count
		1,		// maximum count
		NULL	//_T("{7B86609A-747D-438f-86F5-AA40FC67BE9D}")
		);		// unnamed semaphore
#endif
	m_bParent = false;
}

void CTskTimer::Lock()
{
#ifdef WIN32    
	WaitForSingleObject( m_hPauseEvent, INFINITE);
#endif   
}

void CTskTimer::UnLock()
{
#ifdef WIN32    
	ReleaseSemaphore(m_hPauseEvent,  1,  NULL);
#endif
}

void CTskTimer::Append(CListHead* pListHeader, CTskTimer* p)
{
	p->m_ListEntry.next  = pListHeader->next;
	p->m_ListEntry.prev  = pListHeader; 

	pListHeader->next->prev = &p->m_ListEntry; 
	pListHeader->next       = &p->m_ListEntry; 
}

void CTskTimer::Del(CTskTimer* pTsk)
{
	CListHead* plistEntry = &pTsk->m_ListEntry;
	plistEntry->prev->next = plistEntry->next;
	plistEntry->next->prev = plistEntry->prev;
}

CTskTimer::~CTskTimer(void)
{
	m_nState = STOP;
	UnLock(); 

	if (m_pTskName)
	{
		delete[] m_pTskName;m_pTskName=0;
	}

	if (m_pTime)
	{
		delete m_pTime;m_pTime=0;
	}

	if (m_pNextTrigTime)
	{
		delete m_pNextTrigTime;m_pNextTrigTime=0;
	}

	if (m_pOffset)
	{
		delete m_pOffset;m_pOffset=0;
	}

	if (m_pFreq)
	{
		delete m_pFreq;m_pFreq=0;
	}

	if (m_pActions)
	{
		for (int i=0;i<m_nActionsCount;i++)
		{
			delete m_pActions[i];
		}
		delete[] m_pActions;m_pActions=0;
	}

	if (m_pChildsEntry)
	{
		 CListHead* plistEntry = &m_pChildsEntry->m_ListEntry;
		 for(CListHead* pList = plistEntry->next; pList != plistEntry ; )
		 {
              CTskTimer* p = CONTAINING_RECORD(pList, CTskTimer, m_ListEntry);
			  pList = pList->next;
			  delete p;
		 }
		 delete m_pChildsEntry;
	}
    
#ifdef WIN32    
	::CloseHandle( m_hPauseEvent );
#endif
}
 



