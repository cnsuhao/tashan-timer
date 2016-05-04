

#include "../include/stdafx.h"


#include <FL/fl_ask.H>

#include "TskTimer.h"
#include "TskTimerMgr.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "../utils/intvec.h" //CLongVector
#include "../utils/utils.h"
#include "../include/auto_tchar.h"
#include <time.h>

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

//清除所有子定时器
void CTskTimerMgr::ClearAllTimers(CTskTimer* pTskTimer)
{
	if (!pTskTimer)
	{
		return;
	}

	if (pTskTimer->m_nId != -1)
	{
		//非空头结点
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

		//在子任务链表里查
		if (getTskTimerById(p->m_pChildsEntry,  nId, OUT pTskTimer))
		{
			bFound = true;
			break;
		} 
	}//for

	if (!bFound)
	{
		//没找到的话，在子任务链表里查
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
	//删除任务前，先删除掉任务中的计时器
	ClearAllTimers(pTskTimer);

	//从链表中摘除
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
#endif

	time_t now;
	struct tm *tmNow;
	time(&now);
	tmNow   =   localtime(&now);

    int ntmp=0;
	int Y=0,M=0,D=0,y= tmNow->tm_year,m= tmNow->tm_mon,d=tmNow->tm_mday,h=tmNow->tm_hour,minu=tmNow->tm_min,sec=tmNow->tm_sec;
    
	TCHAR** vtDays=0;
	TCHAR** vtHours=0;

	CParams*   pTime = p->m_pTime;
	CParams*   pFreq = p->m_pFreq;
	int nFreqTimes = 0;
	LONGLONG   nElapse =0, llElapse=0;
	TCHAR tchTmp[MAX_PATH];
 
	if (!pTime)
	{
		//子任务中，该值为NULL.
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
		//pNextTrigTime里的值，会在每次onTimer里面进行修改，并保存到XML里。
		pNextTrigTime = new CParams(*pTime);
	}
 
	//获取XML中保存的，任务下次触发距今的总秒数
	nElapse =  CTskTimerMgr::GetTimeEspase(p->m_pNextTrigTime);
	bool bNextTrigTimeInvalid = false;
	if (nElapse == -1)
	{
		//获取XML中保存的，任务触发时机距今的总秒数
		nElapse =  CTskTimerMgr::GetTimeEspase(p->m_pTime);
		if (nElapse == -1)
		{
			return false;
		}
		bNextTrigTimeInvalid = true;
	} 

	if (p->m_pNextTrigTime->chType == 0)
	{
		//对于即时闹钟，显然下次任务的时间是无效的
		bNextTrigTimeInvalid = true;
	}

	do 
	{
		//获取时间偏移量,参数格式类似于 1.0:0:0
		CParams*   pOffset = p->m_pOffset; 
		int nOffsetDays = 0;

		if (nElapse<0 || bNextTrigTimeInvalid)
		{
			//对于即时闹钟，或者下次触发时机没有正确记录时，我都需要累加下次时间偏移
			llElapse =  CTskTimerMgr::GetOffsetEspase( pOffset);
			nOffsetDays = llElapse/(24*60*60);

			switch(pOffset->chType)
			{
			case 0:
				//之前
				nElapse -= llElapse;
				nOffsetDays = -nOffsetDays;
				break;
			case 1:
				//之后
				nElapse += llElapse;
				break;
			}
		}

		//bNextTrigTimeInvalid只对第一次循环有用
		bNextTrigTimeInvalid = true;

		int nLoopTimes = 0;
		int nTotalDays=0;

		//获取执行频率参数。并据此重新设定计时任务 
		llElapse  =  CTskTimerMgr::GetFreqEspase(pFreq, nElapse);

		int nExecCount = _ttoi(pFreq->m_pParam0);

		//此时 llElapse 存放了频率时段, 
		if ( llElapse >0 && nElapse<0  )
		{ 
			//nExeCount ==1时，任务已经过期，所以必须大于1才处理。
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
                #ifdef _WIN32
				_itot(nExecCount, pFreq->m_pParam0, 10);
                #else
                CParams::CopyParam(pFreq->m_pParam0, l64a(nExecCount));                
                //sprintf(pFreq->m_pParam0, "%d", nExecCount);
                #endif
			} 
		} 

		//y= tmNow.GetYear(),m= tmNow.GetMonth(),d=tmNow.GetDay(),h=tmNow.GetHour(),minu=tmNow.GetMinute(),sec=tmNow.GetSecond();
        y= tmNow->tm_year,m= tmNow->tm_mon,d=tmNow->tm_mday,h=tmNow->tm_hour,minu=tmNow->tm_min,sec=tmNow->tm_sec;
		CTskTimerMgr::GetDateNxtSecs(&y, &m, &d, &h, &minu, &sec, nElapse);

		//ATLASSERT(ISVALIDDATE(y,m,d,h,minu,sec)); 
	
    
		_stprintf(tchTmp,_T("%d.%d.%d"), y, m, d);
		CParams::CopyParam(pNextTrigTime->m_pParam0, tchTmp);

		_stprintf(tchTmp,_T("%d:%d:%d"), h, minu, sec);
		CParams::CopyParam(pNextTrigTime->m_pParam1, tchTmp);
		
		p->m_llTotalElapse = nElapse;
		if (nElapse<0)
		{
			//过期了
			p->m_nState = EXPIRED;
		}

		//让主程序将剩余时间发送给HTML了。
		//::PostMessage(m_hOwnWnd, ngUpdateLeftTimeMsg, p->m_nId,(LPARAM)0 );

		if (nElapse > 3600*24 )
		{
			//超过一天的秒数，那么今天的任务就没有了，所以不需要执行
			return false;
		}

		//::KillTimer(m_hOwnWnd,p->m_nId);
 
		if (nElapse>=0)
		{
			//::SetTimer(m_hOwnWnd, p->m_nId, nElapse*1000, NULL);
			//::PostMessage(m_hOwnWnd, ngTrigTskMsg, p->m_nId,(LPARAM)nElapse );
		}else
		{ 			
			nElapse = -1;//过期的任务
			//对于过期任务，我仍然让它进入onTimer,以便检测其是否有循环或者子任务。

			p->m_nState |= EXPIRED;
		}
        
 		bRtn = true;
        
	} while (0);

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

void CTskTimerMgr::BuildParam(xmlDocPtr doc, IN xmlNodePtr nd, OUT CParams*& p, IN int n)
{
    if(!nd) return;
    
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
		    nLen =  max(MAX_PATH, (int)strlen(key));
		    if(!p->m_pParam0) p->m_pParam0 = (TCHAR*)malloc(sizeof(TCHAR)*(nLen+1));
		    _tcscpy(p->m_pParam0, key);
		}else if (!xmlStrcmp(cur->name, (const xmlChar *)"Param1"))
		{
		    nLen =  max(MAX_PATH, (int)strlen(key));
		    if(!p->m_pParam1) p->m_pParam1 = (TCHAR*)malloc(sizeof(TCHAR)*(nLen+1));
		    _tcscpy(p->m_pParam1, key);
		}else if (!xmlStrcmp(cur->name, (const xmlChar *)"Param2"))
		{
		    nLen =   max(MAX_PATH, (int)strlen(key));
		    if(!p->m_pParam2) p->m_pParam2 = (TCHAR*)malloc(sizeof(TCHAR)*(nLen+1));
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

        const char* key = (const char*)xckey;
		if (!xmlStrcmp(cur->name, (const xmlChar *)"Id"))
		{
            if(key) p->m_nId = atoi(key);
		}
		else if (!xmlStrcmp(cur->name, (const xmlChar *)"State"))
		{
			if(key) p->m_nState = atoi(key);
		}
		else if (!xmlStrcmp(cur->name, (const xmlChar *)"Name"))
		{
            if(!xckey) continue;
            
            nLen = strlen((const char*)key);
            p->m_pTskName = (TCHAR*)malloc(sizeof(TCHAR)*(nLen+1));
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
                    //must use MAX_PATH，main program will reset it's content later.
                    nLen = min((int)strlen((const char*)childKey)+1, (int)MAX_PATH)+1;
                    p->m_pFreq->m_pParam0 = (TCHAR*)malloc(sizeof(TCHAR)*nLen);
                    _tcsncpy(p->m_pFreq->m_pParam0
                             , (const char*)childKey
                             ,nLen);
                }else  if (!xmlStrcmp(pChild->name, (const xmlChar *)"Interval"))
                {
                    nLen = strlen((const char*)childKey)+1;
                    p->m_pFreq->m_pParam1 = (TCHAR*)malloc(sizeof(TCHAR)*nLen);
                    _tcscpy(p->m_pFreq->m_pParam1, (const char*)childKey);
                }
              xmlFree(childKey);
            }//for            
        }
		else if (!xmlStrcmp(cur->name, (const xmlChar *)"Actions"))
		{
            CLongVector lv;
            for(pChild = cur->xmlChildrenNode;pChild;pChild=pChild->next)
            {
                if (xmlStrcmp(pChild->name, (const xmlChar *)"Action")) continue;
                childKey = xmlNodeListGetString(doc, pChild->xmlChildrenNode, 1);
                
                CParams* pAcParam =  new CParams;
                BuildParam(doc, IN pChild, OUT pAcParam, 1);
                //if pActionParams->m_pParam0's file not exist ,then I generate it's related fn
                if(access(pAcParam->m_pParam0,0)==-1)
                {
                    if(!pAcParam->m_pParam2) pAcParam->m_pParam2 = (TCHAR*)malloc(MAX_PATH*sizeof(TCHAR));
                    if(!nsYLX::CUtil::GetRelatedName(pAcParam->m_pParam0, pAcParam->m_pParam2, MAX_PATH*sizeof(TCHAR)))
                    {
                        pAcParam->m_pParam2[0]=0;
                    }        
                }                
                lv.push_back((long)pAcParam);
                
                if(childKey) xmlFree(childKey);
            }//for
            
            p->m_nActionsCount = lv.size();
            if(p->m_nActionsCount>0)
            {
                p->m_pActions = new CParams*[p->m_nActionsCount];
                for(int k=0;k<p->m_nActionsCount;k++)
                {
                    p->m_pActions[k] = (CParams*)lv[k];
                }//for
            }//if
        }
		else if (!xmlStrcmp(cur->name, (const xmlChar *)"Tasks"))
		{
          p->m_pChildsEntry= new CTskTimer;
          ParseTsksNode(doc, cur, OUT p->m_pChildsEntry);
		}
                
        if(xckey) xmlFree(xckey);
	}//for2
}//XmlNodeToTsk

bool CTskTimerMgr::ParseTsksNode(xmlDocPtr doc, xmlNodePtr tsksNd, OUT  CTskTimer* pTskTimerEntry)
{
    for (xmlNodePtr ndTsk = tsksNd->xmlChildrenNode;ndTsk;ndTsk=ndTsk->next)
	{
		if (xmlStrcmp(ndTsk->name, (const xmlChar *)"Task")) continue;
    
        CTskTimer* p = new CTskTimer;
        XmlNodeToTsk(doc, IN ndTsk,IN OUT p);

        //p->m_bParent = bAsParent;
        //倒序加入到任务链表 
        p->m_ListEntry.next  = pTskTimerEntry->m_ListEntry.next;
        p->m_ListEntry.prev  = &pTskTimerEntry->m_ListEntry; 
 
        pTskTimerEntry->m_ListEntry.next->prev = &p->m_ListEntry; 
        pTskTimerEntry->m_ListEntry.next       = &p->m_ListEntry;
	}//for1
    return true;
}

bool CTskTimerMgr::LoadTskXml(const char* resFile)
{
	xmlDocPtr doc=0;   //定义解析文档指针
	xmlNodePtr root;  //定义结点指针(你需要它为了在各个结点间移动)
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
	root = xmlDocGetRootElement(doc);  //确定文档根元素

    ParseTsksNode(doc, root, OUT m_pTskTimerEntry);
    
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

		//先找任务
		JWXml::CXmlNodes ndTsks;
		root->SelectNodes(&ndTsks, _T("Tasks/Task "));//Tasks/Task 

		ParseChildXml(&ndTsks, m_pTskTimerEntry,true);
	} while (0);

	delete root;
	delete pTmpXml;
    #endif
}

