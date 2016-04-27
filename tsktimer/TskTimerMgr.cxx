

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
                //must use MAX_PATH，main program will reset it's content later.
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

		//先找任务
		JWXml::CXmlNodes ndTsks;
		root->SelectNodes(&ndTsks, _T("Tasks/Task "));//Tasks/Task 

		ParseChildXml(&ndTsks, m_pTskTimerEntry,true);
	} while (0);

	delete root;
	delete pTmpXml;
    #endif
}

