#pragma once

#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#else
#include <stdio.h>
#include "../include/auto_tchar.h"
#endif

#include "../include/macrodef.h"

class  CParams
{
public:
	CParams();
	CParams(const CParams& other);

	~CParams();
	 int	m_nId;
	 char   chType;
	 TCHAR* m_pParam0;
	 TCHAR* m_pParam1;
	 TCHAR* m_pParam2; 

	 static void CopyParam(TCHAR*& pDes, TCHAR* pSrc);
	 CParams& operator=(const CParams& other);
};

//Backpack list
struct CListHead
{ 
	struct CListHead *prev,*next;
	CListHead()
	{
		prev = next = this;
	};
	//CListHead& operator=(const CListHead& other);
};

enum TSKSTATE
{
    RUNNING		=	0x1,    //��������
	STOP		=	0x2,
	PAUSE		=	((int)0x1)<<2,      //��ͣ
	FORDELETE	=	((int)0x1)<<3,  //��ɾ��
	EXPIRED		=	((int)0x1)<<4     //����
};

class CTskTimer
{
public: 
	CListHead   m_ListEntry;

	CTskTimer(void);
	~CTskTimer(void);
 
	int			m_nId;
	int         m_nActionsCount;
	int         m_nState;//�ñ�־��OnTimer�����ʵ��ִ��ʱ������ 0:δִ��   1:��ͣ    2:���ڻ���Ч���ȴ�ɾ��

	LONGLONG    m_llTotalElapse;  //���񴥷������������
	bool        m_bParent;

	TCHAR*		m_pTskName;
	CParams*    m_pTime;
	CParams*    m_pNextTrigTime;

	CParams*	m_pOffset;
	CParams*	m_pFreq;
	CParams**	m_pActions;
	CTskTimer*  m_pChildsEntry;


	static void Append(CListHead* pListHeader, CTskTimer* pTsk);
	static void Del(CTskTimer* pTsk);

	HANDLE      m_hPauseEvent;
	void Lock();
	void UnLock();

};
