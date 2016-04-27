

#include "../include/stdafx.h"


#include <FL/fl_ask.H>

#include "TskTimer.h"
#include "TskTimerMgr.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
//#include <linux/kernel.h>


#if defined(WIN32) && defined(_DEBUG)
#define new DEBUG_NEW
#endif


CTskTimerMgr::CTskTimerMgr()
{
	m_pTskTimerEntry = new CTskTimer;

}

CTskTimerMgr::~CTskTimerMgr(void)
{
	if (m_pTskTimerEntry)
	{
		CListHead* plistEntry = &m_pTskTimerEntry->m_ListEntry;
		for(CListHead* pList = plistEntry->next; pList != plistEntry ; )
		{
			CTskTimer* p = CONTAINING_RECORD(pList, CTskTimer, m_ListEntry);
			pList = pList->next;
			delete p;
		}
		delete m_pTskTimerEntry;
	} 
}

//��������Ӷ�ʱ��
void CTskTimerMgr::ClearAllTimers(CTskTimer* pTskTimer)
{
	if (!pTskTimer)
	{
		return;
	}

	if (pTskTimer->m_nId != -1)
	{
		//�ǿ�ͷ���
		//::KillTimer(m_hOwnWnd, pTskTimer->m_nId); 
		ClearAllTimers(pTskTimer->m_pChildsEntry);
	}

	CListHead* plistEntry = &pTskTimer->m_ListEntry;

	for(CListHead* pList = plistEntry->next; pList != plistEntry ; pList = pList->next)
	{
		CTskTimer* p = CONTAINING_RECORD(pList, CTskTimer, m_ListEntry);

		//::KillTimer(m_hOwnWnd, p->m_nId);
		ClearAllTimers(p->m_pChildsEntry);
	}
}//ClearAllTimers

bool CTskTimerMgr::getTskTimerById(IN CTskTimer* pTskTimerEntry, IN int nId, OUT CTskTimer** pTskTimer)
{
	bool bFound  = false;
	if (!pTskTimerEntry || *pTskTimer != NULL )
	{
		return bFound;
	}

	if (pTskTimerEntry->m_nId == nId)//
	{
		*pTskTimer = pTskTimerEntry;
		bFound = true;
		return bFound;
	}

	CListHead* plistEntry = &pTskTimerEntry->m_ListEntry;
	CListHead* pList = plistEntry->next;

	for(; pList != plistEntry ; )
	{
		CTskTimer* p = CONTAINING_RECORD(pList, CTskTimer, m_ListEntry);

		pList = pList->next;

		if (p->m_nId == nId)
		{
			*pTskTimer = p;
			bFound = true;
			break;
		}

		//���������������
		if (getTskTimerById(p->m_pChildsEntry,  nId, OUT pTskTimer))
		{
			bFound = true;
			break;
		} 
	}//for

	if (!bFound)
	{
		//û�ҵ��Ļ������������������
		if (getTskTimerById(pTskTimerEntry->m_pChildsEntry,  nId, OUT pTskTimer))
		{
			return bFound;
		} 
	}
	return bFound;
}//CTskTimerMgr::getTskTimerById

bool CTskTimerMgr::DelTskTimerById(int nId)
{	
	bool bRtn = false;
	CTskTimer* pTskTimer=0;
	if (!getTskTimerById(m_pTskTimerEntry,nId,&pTskTimer))
	{
		return bRtn;
	}
	//ɾ������ǰ����ɾ���������еļ�ʱ��
	ClearAllTimers(pTskTimer);

	//��������ժ��
	CTskTimer::Del(pTskTimer); 
	
	delete pTskTimer;

	return bRtn;
}

bool CTskTimerMgr::DelTskTimer(IN BYTE* psXml)
{
	bool bRtn = false;


	return bRtn;
}//DelTskTimer


bool CTskTimerMgr::AddTskTimers(IN BYTE* psXml, IN CTskTimer* pTskTimerEntry)
{
	bool bRtn = false;

	if (!psXml)
	{
		return bRtn;
	}


	return bRtn;
}//bool CTskTimerMgr::AddTskTimers(IN BYTE* psXml, IN CTskTimer* pTskTimerEntry)

bool CTskTimerMgr::AddTskTimer(IN BYTE* psXml, IN CTskTimer* pTskTimerEntry ,bool bAsParent)
{
	bool bRtn = false;

	if (!psXml)
	{
		return bRtn;
	}

	return true;
}//bool CTskTimerMgr::AddTskTimer(IN BYTE* psXml)

bool CTskTimerMgr::StartSingleTimerTsk(CTskTimer* p)
{
	bool bRtn = false;
	if (!p)
	{
		return false;
	}
#if 0
	CTimeSpan ts;
	CTime   tmNow( 0 ),ct(0) ;
	tmNow = CTime::GetCurrentTime();
	int Y=0,M=0,D=0,y= tmNow.GetYear(),m= tmNow.GetMonth(),d=tmNow.GetDay(),h=tmNow.GetHour(),minu=tmNow.GetMinute(),sec=tmNow.GetSecond();

	TCHAR** vtDays=0;
	TCHAR** vtHours=0;

	CParams*   pTime = p->m_pTime;
	CParams*   pFreq = p->m_pFreq;
	int nFreqTimes = 0;//_ttoi(pFreq->m_pParam0);
	LONGLONG   nElapse =0, llElapse=0;
	TCHAR tchTmp[MAX_PATH];
 
	if (!pTime)
	{
		//�������У���ֵΪNULL.
		pTime = new CParams();

		_stprintf(tchTmp,_T("%d.%d.%d"), y, m, d);
		CParams::CopyParam(pTime->m_pParam0, tchTmp);

		_stprintf(tchTmp,_T("%d:%d:%d"), h, minu, sec);
		CParams::CopyParam(pTime->m_pParam1, tchTmp);

		p->m_pTime = pTime;
	}

	CParams*   pNextTrigTime = p->m_pNextTrigTime;
	if (!pNextTrigTime)
	{
		//pNextTrigTime���ֵ������ÿ��onTimer��������޸ģ������浽XML�
		pNextTrigTime = new CParams(*pTime);
	}
 
	//��ȡXML�б���ģ������´δ�������������
	nElapse =  CTskTimerMgr::GetTimeEspase(p->m_pNextTrigTime);
	bool bNextTrigTimeInvalid = false;
	if (nElapse == -1)
	{
		//��ȡXML�б���ģ����񴥷�ʱ������������
		nElapse =  CTskTimerMgr::GetTimeEspase(p->m_pTime);
		if (nElapse == -1)
		{
			return false;
		}
		bNextTrigTimeInvalid = true;
	} 

	if (p->m_pNextTrigTime->chType == 0)
	{
		//���ڼ�ʱ���ӣ���Ȼ�´������ʱ������Ч��
		bNextTrigTimeInvalid = true;
	}

	do 
	{
		//��ȡʱ��ƫ����,������ʽ������ 1.0:0:0
		CParams*   pOffset = p->m_pOffset; 
		int nOffsetDays = 0;

		if (nElapse<0 || bNextTrigTimeInvalid)
		{
			//���ڼ�ʱ���ӣ������´δ���ʱ��û����ȷ��¼ʱ���Ҷ���Ҫ�ۼ��´�ʱ��ƫ��
			llElapse =  CTskTimerMgr::GetOffsetEspase( pOffset);
			nOffsetDays = llElapse/(24*60*60);

			switch(pOffset->chType)
			{
			case 0:
				//֮ǰ
				nElapse -= llElapse;
				nOffsetDays = -nOffsetDays;
				break;
			case 1:
				//֮��
				nElapse += llElapse;
				break;
			}
		}

		//bNextTrigTimeInvalidֻ�Ե�һ��ѭ������
		bNextTrigTimeInvalid = true;
#if 0
		int newY=0, newM=0, newD=0;

		//������ʽ������  2013.2.24 
		newY=nsYLX::CUtil::Split(OUT vtDays, IN 3, IN  pTime->m_pParam0, IN _T("."));
		if (newY!=3)
		{
			nsYLX::CUtil::ClearMemForSplit(vtDays,3);
			return -1;//��ʽ����ѽ
		}

		Y = _ttoi(vtDays[0]);
		M = _ttoi(vtDays[1]);
		D = _ttoi(vtDays[2]);
		nsYLX::CUtil::ClearMemForSplit(vtDays,3);

		//����ƫ�Ƶ������������һ�����ڵ�������
		CTskTimerMgr::GetDateNxtDays(Y, M, D, newY, newM, newD, nOffsetDays);

		//����ƫ�Ƶ������������һ�����ڵ�ʱ����
		nOffsetDays = llElapse%(24*60*60);
		if (pOffset->chType == 0)
		{
			//֮ǰ
			nOffsetDays -= nOffsetDays;
		}

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
		CTskTimerMgr::GetDateNxtSecs(&newY, &newM, &newD, &h, &minu, &sec, nOffsetDays);
 
		_stprintf(tchTmp,_T("%d.%d.%d"), newY, newM, newD);
		CParams::CopyParam(pNextTrigTime->m_pParam0, tchTmp);

		_stprintf(tchTmp,_T("%d:%d:%d"), h, minu, sec);
		CParams::CopyParam(pNextTrigTime->m_pParam1, tchTmp);
#endif

		int nLoopTimes = 0;
		int nTotalDays=0;

		//��ȡִ��Ƶ�ʲ��������ݴ������趨��ʱ���� 
		llElapse  =  CTskTimerMgr::GetFreqEspase(pFreq, nElapse);

		int nExecCount = _ttoi(pFreq->m_pParam0);

		//��ʱ llElapse �����Ƶ��ʱ��, 
		if ( llElapse >0 && nElapse<0  )
		{ 
			//nExeCount ==1ʱ�������Ѿ����ڣ����Ա������1�Ŵ���
			if (nExecCount==-1)
			{
				nElapse = llElapse -  (-nElapse) % llElapse; 
			}else
			{
				while( nExecCount>1 )
				{
					nElapse += llElapse;
					if (nElapse>=0)
					{
						break;
					}
					nExecCount--;
				}
				_itot(nExecCount, pFreq->m_pParam0, 10);
			} 
		} 

		y= tmNow.GetYear(),m= tmNow.GetMonth(),d=tmNow.GetDay(),h=tmNow.GetHour(),minu=tmNow.GetMinute(),sec=tmNow.GetSecond();
		CTskTimerMgr::GetDateNxtSecs(&y, &m, &d, &h, &minu, &sec, nElapse);

		ATLASSERT(ISVALIDDATE(y,m,d,h,minu,sec)); 

		_stprintf(tchTmp,_T("%d.%d.%d"), y, m, d);
		CParams::CopyParam(pNextTrigTime->m_pParam0, tchTmp);

		_stprintf(tchTmp,_T("%d:%d:%d"), h, minu, sec);
		CParams::CopyParam(pNextTrigTime->m_pParam1, tchTmp);
		
		p->m_llTotalElapse = nElapse;
		if (nElapse<0)
		{
			//������
			p->m_nState = EXPIRED;
		}

		//��������ʣ��ʱ�䷢�͸�HTML�ˡ�
		::PostMessage(m_hOwnWnd, ngUpdateLeftTimeMsg, p->m_nId,(LPARAM)0 );

		if (nElapse > 3600*24 )
		{
			//����һ�����������ô����������û���ˣ����Բ���Ҫִ��
			return false;
		}

		::KillTimer(m_hOwnWnd,p->m_nId);
 
		if (nElapse>=0)
		{
			//::SetTimer(m_hOwnWnd, p->m_nId, nElapse*1000, NULL);
			::PostMessage(m_hOwnWnd, ngTrigTskMsg, p->m_nId,(LPARAM)nElapse );
		}else
		{ 			
			nElapse = -1;//���ڵ�����
			//���ڹ�����������Ȼ��������onTimer,�Ա������Ƿ���ѭ������������

			p->m_nState |= EXPIRED;
		}
	 
		bRtn = true;

	} while (0);
#endif
	return bRtn;
}//StartSingleTimerTsk
 
void CTskTimerMgr::StartTimerTsk(CListHead* plistEntry)
{
	if (!plistEntry)
	{
		return;
	}

	for(CListHead* pList = plistEntry->prev; pList != plistEntry ;pList = pList->prev)
	{
		CTskTimer* p = CONTAINING_RECORD(pList, CTskTimer, m_ListEntry);

		if (!StartSingleTimerTsk(p))
		{
			continue;
		}  
	}//for
}//StartTimerTsk

void CTskTimerMgr::BuildParam(xmlDocPtr doc, IN xmlNodePtr nd, OUT CParams* p, IN int n)
{
    if(!p) p = new CParams();
    xmlChar* xckey;
    int nLen=0;
	for(xmlNodePtr cur = nd->xmlChildrenNode;cur;cur=cur->next)
	{
		xckey = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        if(!xckey) continue;
        const char* key = (const char*)xckey;
        
        if (!xmlStrcmp(cur->name, (const xmlChar *)"Id"))
		{
		    p->m_nId  = atoi(key);
		}else if (!xmlStrcmp(cur->name, (const xmlChar *)"Type"))
		{
		    p->chType  = atoi(key);
		}else if (!xmlStrcmp(cur->name, (const xmlChar *)"Param0")
                ||!xmlStrcmp(cur->name, (const xmlChar *)"Param"))
		{
		    nLen =   strlen(key);
		    if(!p->m_pParam0) p->m_pParam0 = new TCHAR[nLen+1];
		    _tcscpy(p->m_pParam0, key);
		}else if (!xmlStrcmp(cur->name, (const xmlChar *)"Param1"))
		{
		    nLen =   strlen(key);
		    if(!p->m_pParam1) p->m_pParam1 = new TCHAR[nLen+1];
		    _tcscpy(p->m_pParam1, key);
		}else if (!xmlStrcmp(cur->name, (const xmlChar *)"Param2"))
		{
		    nLen =   strlen(key);
		    if(!p->m_pParam2) p->m_pParam2 = new TCHAR[nLen+1];
		    _tcscpy(p->m_pParam2, key);
		}     
        xmlFree(xckey);
    }//for
}

void CTskTimerMgr::XmlNodeToTsk(xmlDocPtr doc, IN xmlNodePtr ndTsk,IN OUT CTskTimer* p)
{
	if (!ndTsk || !p)
	{
		return;
	}
    int nLen=0;
    xmlNodePtr pChild;
    xmlChar* xckey, *childKey; 
	for(xmlNodePtr cur = ndTsk->xmlChildrenNode;cur;cur=cur->next)
	{
		xckey = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        if(!xckey) continue;
        const char* key = (const char*)xckey;
		if (!xmlStrcmp(cur->name, (const xmlChar *)"Id"))
		{
            p->m_nId = atoi(key);
		}
		else if (!xmlStrcmp(cur->name, (const xmlChar *)"State"))
		{
			p->m_nState = atoi(key);
		}
		else if (!xmlStrcmp(cur->name, (const xmlChar *)"Name"))
		{
            nLen = strlen((const char*)key);
            p->m_pTskName = new TCHAR[nLen+1];
            _tcscpy(p->m_pTskName,  (const char*)key);
            
		}
		else if (!xmlStrcmp(cur->name, (const xmlChar *)"Time"))
		{
            BuildParam(doc, IN cur, OUT p->m_pTime, 2);          
		}
		else if (!xmlStrcmp(cur->name, (const xmlChar *)"NextTrigTime"))
		{
            BuildParam(doc, IN cur, OUT p->m_pNextTrigTime, 2);          
        }
		else if (!xmlStrcmp(cur->name, (const xmlChar *)"Offset"))
		{
            BuildParam(doc, IN cur, OUT p->m_pOffset, 1); 
        }
		else if (!xmlStrcmp(cur->name, (const xmlChar *)"Freq"))
		{
            for(pChild = cur->xmlChildrenNode;pChild;pChild=pChild->next)
            {
                childKey = xmlNodeListGetString(doc, pChild->xmlChildrenNode, 1);
                if(!childKey) continue;
                
                if (!xmlStrcmp(pChild->name, (const xmlChar *)"Type"))
                {
                    p->m_pFreq->chType  = atoi((const char*)childKey);
                }else  if (!xmlStrcmp(pChild->name, (const xmlChar *)"Times"))
                {
                    //must use MAX_PATH��main program will reset it's content later.
                    p->m_pFreq->m_pParam0 = new TCHAR[MAX_PATH];
                    _tcsncpy(p->m_pFreq->m_pParam0
                             , (const char*)childKey
                             ,min((int)strlen((const char*)childKey)+1, (int)MAX_PATH));
                }else  if (!xmlStrcmp(pChild->name, (const xmlChar *)"Interval"))
                {
                    nLen = strlen((const char*)childKey);
                    p->m_pFreq->m_pParam1 = new TCHAR[nLen+1];
                    _tcscpy(p->m_pFreq->m_pParam1, (const char*)childKey); 
                }
                xmlFree(childKey);
            }//for            
        }
		else if (!xmlStrcmp(cur->name, (const xmlChar *)"Actions"))
		{
            
            BuildParam(doc, IN cur, OUT p->m_pOffset, 1); 
        }
		else if (!xmlStrcmp(cur->name, (const xmlChar *)"Tasks"))
		{
       		p->m_pChildsEntry= new CTskTimer;
            ParseTsksNode(doc, cur, OUT p->m_pChildsEntry);
		}
                
        xmlFree(xckey);
	}//for2
}//XmlNodeToTsk

//pTskTimerEntry !=NULL 
void CTskTimerMgr::ParseChildXml(xmlNodePtr ndTsks,IN OUT CTskTimer* pTskTimerEntry, IN bool bAsParent)
{
	if (!pTskTimerEntry)
	{
		return;
	}
}//ParseChildXml

bool CTskTimerMgr::ParseTsksNode(xmlDocPtr doc, xmlNodePtr tsksNd, OUT  CTskTimer* pTskTimerEntry)
{
    for (xmlNodePtr ndTsk = tsksNd->xmlChildrenNode;ndTsk;ndTsk=ndTsk->next)
	{
		if (xmlStrcmp(ndTsk->name, (const xmlChar *)"Task")) continue;
    
        CTskTimer* p = new CTskTimer;
        XmlNodeToTsk(doc, IN ndTsk,IN OUT p);

        //p->m_bParent = bAsParent;
        //������뵽�������� 
        p->m_ListEntry.next  = pTskTimerEntry->m_ListEntry.next;
        p->m_ListEntry.prev  = &pTskTimerEntry->m_ListEntry; 
 
        pTskTimerEntry->m_ListEntry.next->prev = &p->m_ListEntry; 
        pTskTimerEntry->m_ListEntry.next       = &p->m_ListEntry;
	}//for1
    return true;
}

bool CTskTimerMgr::LoadTskXml(const char* resFile)
{
	xmlDocPtr doc=0;   //��������ĵ�ָ��
	xmlNodePtr root;  //������ָ��(����Ҫ��Ϊ���ڸ��������ƶ�)
	bool ret = true;

	if(access(resFile,0)==-1)
	{
        fl_alert("%s not exist.", resFile);        
		return false;
	}
	xmlKeepBlanksDefault(0);
	xmlIndentTreeOutput = 1 ;// indent .with \n

	doc = xmlReadFile(resFile, "UTF-8", XML_PARSE_NOBLANKS | XML_PARSE_NODICT);
	if (doc == NULL )
	{
        fl_alert("%s not parsed successfully.", resFile);
		return 0;
	}    
	root = xmlDocGetRootElement(doc);  //ȷ���ĵ���Ԫ��
    xmlNodePtr tsksNd = root;
    ParseTsksNode(doc, tsksNd, OUT m_pTskTimerEntry);
    
	xmlFreeDoc(doc);
	xmlCleanupParser();     
    return ret;
}//loadTskXml

void CTskTimerMgr::ParseXml(BYTE* psXml)
{
	if (!psXml)
	{
		return ;
	}
    #if 0
	JWXml::CXml* pTmpXml = new JWXml::CXml();

	BOOL isXmlFile = pTmpXml->LoadXml((BSTR)psXml);

	//ATLASSERT(isXmlFile);
	if (!isXmlFile)
	{
	    delete pTmpXml;
		return;
	}

	JWXml::CXmlNodePtr root=new JWXml::CXmlNode();
	pTmpXml->GetRoot(root); 

	do 
	{
		if( root->IsNull() ) 
		{	
			break;
		}
		CString s=	root->GetValue();

		//��������
		JWXml::CXmlNodes ndTsks;
		root->SelectNodes(&ndTsks, _T("Tasks/Task "));//Tasks/Task 

		ParseChildXml(&ndTsks, m_pTskTimerEntry,true);
	} while (0);

	delete root;
	delete pTmpXml;
    #endif
}

