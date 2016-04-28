#pragma once

#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#else
#include <stdio.h>
#include "../include/auto_tchar.h"
#endif

#include <libxml/parser.h>
#include <libxml/tree.h>

class  CTskTimerMgr
{
public:
	CTskTimerMgr();
	~CTskTimerMgr(void);

	class CTskTimer*	m_pTskTimerEntry;
    
	bool getTskTimerById(IN CTskTimer* pTskTimerEntry, IN int nId, OUT CTskTimer** pTskTimer);
	void ClearAllTimers(CTskTimer* pTskTimer);

	bool AddTskTimers(IN BYTE* psXml, IN CTskTimer* pTskTimerEntry);
	bool AddTskTimer(IN BYTE* psXml, IN CTskTimer* pTskTimerEntry,bool bAsParent);
	bool DelTskTimer(IN BYTE* psXml);
	bool DelTskTimerById(int nId);
    
    void BuildParam(xmlDocPtr doc, IN xmlNodePtr nd, OUT CParams* p, IN int n);
	void XmlNodeToTsk(xmlDocPtr doc, IN  xmlNodePtr ndTsk,IN OUT CTskTimer* p);

	void ParseXml(BYTE* psXml);
    bool LoadTskXml(const char* resFile);
    
	bool StartSingleTimerTsk(CTskTimer* p);
	void StartTimerTsk(class CListHead* plistEntry);
    bool ParseTsksNode(xmlDocPtr doc, xmlNodePtr tsksNd, OUT  CTskTimer* pTskEntry);
    
	static LONGLONG GetTimeEspase(class CParams* pOffset);
	static LONGLONG GetFreqEspase(class CParams* pFreq, const LONGLONG llTotalSecsOffsetNow);
	static LONGLONG GetOffsetEspase(class CParams* pFreq);
	static bool     CanDelTsk(IN CTskTimer* pTskTimerEntry);
	static int      GetDaysOfCurrMonth(int nMonth, int nYear);
	static bool     GetDateNxtDays(int y,int m,int d,int& newY,int& newM,int& newD,int nOffsetDays);
	static bool     GetDateNxtSecs(int* pY, int* pMon, int* pD, int* pH, int* pMin, int* pS, int nOffsetSecs);

};
