

#include"mainframe.h"
#include <FL/fl_ask.H>

#include "test/pixmaps/tile.xpm"
#include "about_panel.h"
#include "newtask_frame.h"
#include "taskframe.h"
#include <assert.h>
#include "utils/utils.h"
#include "tsktimer/TskTimer.h"
#include "tsktimer/TskTimerMgr.h"
#include "tasktable.h"

void newtsk_cb(Fl_Widget *w, void *)
{
    Fl_Group* fg = (Fl_Group*)w->parent();
	CMainFrame* pThis = (CMainFrame*)(w->parent()->parent()->parent()->parent());
	if(!pThis) return;
	//fl_alert("you have press %s, %s", w->label(), pThis->label());

    if(pThis->pTskAddFrame) delete pThis->pTskAddFrame;
	pThis->pTskAddFrame = new CNewTaskFrame(345, 475, "新建定时任务");
    CNewTaskFrame*  pWnd = (CNewTaskFrame*)pThis->pTskAddFrame;

    nsYLX::CUtil::CenterWnd(pWnd, pThis);

    CTskTimer* pTskTimer = new CTskTimer;

#if 0
	TCHAR*		m_pTskName;
	CParams*    m_pTime;
	CParams*    m_pNextTrigTime;

	CParams*	m_pOffset;
	CParams*	m_pFreq;
	CParams**	m_pActions;
	CTskTimer*  m_pChildsEntry;
#endif
    struct
    {
        const char* artskName;
        int chType;
    }tm[]=
         {
             {"关机",2},
             {"锁屏",6},             
             {"待机",0},
             {"休眠",1},
             {"护眼",6}
         };
 
    pTskTimer->m_nActionsCount = 1;
    pTskTimer->m_pActions = (CParams**)new CParams*[pTskTimer->m_nActionsCount];
    CParams *pAc = new CParams();
    pTskTimer->m_pActions[0] = pAc;
    //reset  ctrl's content of pTskAddFrame
    int n = fg->find(w);
    
    CParams::CopyParam(pTskTimer->m_pTskName, tm[n].artskName);
    pAc->chType = tm[n].chType;


    CTaskTable* pTskTbl =  pThis->m_pTskFrame->m_pTskTable;
    int nCurrSel = pTskTbl->GetSelRow();
    pTskTbl->AddRow(pThis->pTskAddFrame, pTskTimer, nCurrSel);

}//newtsk_cb

