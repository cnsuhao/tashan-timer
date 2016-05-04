
#include "include/stdafx.h"
#include "tsktimer/TskTimer.h"
#include "tsktimer/TskTimerMgr.h"
#include "taskrow.h"
#include "tasktable.h"
#include "taskframe.h"
#include "ui/imgdata.h"
#include "utils/utils.h"
#include <FL/fl_ask.H>
#include "newtask_frame.h"
#include "mainframe.h"

extern CBtnStruc btnsStruc[];

CTaskRow* CTaskTable::BuildTskRow(int& yOffset, CTskTimer* pTskTimer)
{ 
    CTaskRow* oRow =0;

	oRow =  new CTaskRow(0, yOffset,w(), LINEHEIGHT, "" ,0);
	yOffset=oRow->BuildRow(1, yOffset, LINEHEIGHT, btnsStruc);
    oRow->m_pTskTimer = pTskTimer;
	oRow->end();
		
	//modify tsk name
    if(pTskTimer)
    {
        Fl_Group * gp0 = (Fl_Group*)oRow->child(0);
        Fl_Widget* tskTitle =  (Fl_Widget*)gp0->child(1);

        tskTitle->copy_label(pTskTimer->m_pTskName);
    }

	return oRow;
}//BuildTskRow


void CTaskTable::swapRow(CTaskRow*& a, CTaskRow*& b)
{
    if(a==b) return;
    
	int na = find(a);
	int nb = find(b);
    
	if(na<nb)
	{      
     	remove(na);
		remove(nb-1); 
		insert(*b, na);
		insert(*a, nb);
	}else if(na>nb)
	{
     	remove(nb);
		remove(na-1); 
		insert(*a, nb);
		insert(*b, na);
	}
    
    int x = a->x(),y=a->y();
    a->position(b->x(),b->y());
    b->position(x,y);
    vtRows.swap(na,nb);

    HighLight(na);
}

void CTaskTable::HighLight(int n)
{
	for(int i=0,nCount=children()-1;i<nCount;i++)
	{
		CTaskRow* pRow = (CTaskRow*)child(i);
        if(i==n) pRow->m_bLeftBtnDown=true;
		else pRow->m_bLeftBtnDown=false;
	}//for
    redraw();
}

void CTaskTable::OnMoveUpDown(Fl_Widget *w ,void* p)
{
    Fl_Group* fg = (Fl_Group*)w->parent();
	CTaskRow* oRow = (CTaskRow*)fg->parent()->parent();
    CTaskTable* pThis = (CTaskTable*)oRow->parent();
	int nLine = pThis->find(oRow);
    
    if(fg->find(w)==2)//(strcmp((const char*)w->tooltip(), btnsStruc[2].tips)==0)
	{
        CTaskRow* a= (CTaskRow*)pThis->child(nLine-1);
        //move up
		if(nLine>0)
			pThis->swapRow(a, oRow);
	}else if(fg->find(w)==3)
	{
        CTaskRow* a = (CTaskRow*)pThis->child(nLine+1);
	    //move down
		if(nLine<pThis->children()-2)
			pThis->swapRow(a, oRow);
	}
}

void CTaskTable::OnTskPause(Fl_Widget *w, void *)
{
	CTaskRow* oRow = (CTaskRow*)w->parent()->parent()->parent();

    if(!oRow->m_pTskTimer) return;
	int *nState = &oRow->m_pTskTimer->m_nState;
	Fl_Image* acIcon=0;

	if((*nState)&PAUSE)
	{
		acIcon = nsYLX::CUtil::copy(oRow->m_icons,btnsStruc[7].pt.x, btnsStruc[7].pt.y,18,18);
		*nState &= ~PAUSE;
		*nState |= RUNNING;
	}else
	{
		acIcon = nsYLX::CUtil::copy(oRow->m_icons,btnsStruc[4].pt.x, btnsStruc[4].pt.y,18,18);
		*nState &= ~RUNNING;
		*nState |= PAUSE;
	}
	w->image(acIcon);
}//OnTskPause

CTaskRow* _insertRow(CTaskTable* pThis, CTskTimer* pTskTimer, int nLine)
{
    //nLine must between [-1, pThis->children()-1), because the last row of CTaskTable is a Fl_Group for resizing
    if(nLine > pThis->children()-2)
        nLine = pThis->children()-2;
    else if(nLine<-1)
        nLine = -1;
    
    CTaskRow *oRow = (CTaskRow*)pThis->child(nLine);
    if(!oRow)
        oRow = (CTaskRow*)((Fl_Group*)pThis->resizable())->child(0);
    
    CTaskFrame* pTskFrame =(CTaskFrame*)pThis->parent()->parent(); 
    const int nLineSpace = 5;

    pThis->m_nRowsHeightTotal +=  LINEHEIGHT + nLineSpace;
    pTskFrame->AdjustTskTableHeight();

    int yOffset =  1;
    if(oRow && pThis->children()>1)
        yOffset = oRow->y() + oRow->h() + nLineSpace;
    
	CTaskRow* oNewRow =  new CTaskRow(0, yOffset, pThis->w(), LINEHEIGHT,"",0);
    yOffset=oNewRow->BuildRow(1, oNewRow->y(),  LINEHEIGHT, btnsStruc);
	oNewRow->end();
    oNewRow->m_pTskTimer = pTskTimer;

	//adjust row's y() between nLine+1 and lines    
    CTaskRow* oNext =0;
    for(int i = nLine+1,nCount= pThis->vtRows.size();i<nCount;i++)
    {
        oNext = (CTaskRow*)pThis->child(i);
        if(!oNext) break;
        oNext->position(oNext->x(),oNext->y() + LINEHEIGHT + nLineSpace);
    }
         
	pThis->insert(*oNewRow, nLine+1);
 
	//adjust oNewRow's all cells's w()
    if(oRow)
    {
        for(int i=1;i<4;i++)
        {
            Fl_Widget* oTaskNamePrev = ((Fl_Group*)oRow->child(0))->child(i);      
            Fl_Widget* oNewTaskName = ((Fl_Group*)oNewRow->child(0))->child(i);
            oNewTaskName->resize(oTaskNamePrev->x(),
								 oNewTaskName->y(),
								 oTaskNamePrev->w(),
								 oNewTaskName->h());
        }
    }

    pThis->vtRows.insert((long)oNewRow, nLine);
    pTskFrame->AdjustTskTableHeight();
    return oNewRow;
}

CTaskRow* CTaskTable::AddRow(CNewTaskFrame*  pNewTskFrm, CTskTimer* pTskTimer, int nLine)
{
    //reset  ctrl's content of pTskAddFrame
    pNewTskFrm->BuildTskEditWnd(pTskTimer);
  
    pNewTskFrm->show();
    while(Fl::modal() == pNewTskFrm)
    {
        Fl::wait();
    }
    
    if(pNewTskFrm->m_nRslt!=0)
    {
        delete pTskTimer;
        return 0;
    }
    
    CTaskRow* oNewRow =  _insertRow(this, pTskTimer, nLine);
    
    //update ui
    oNewRow->UpdateUI();
    return oNewRow;
}

void CTaskTable::OnAddRow(Fl_Widget *w, void *)
{
	CTaskRow* oRow = (CTaskRow*)w->parent()->parent()->parent();
	CTaskTable* pThis = (CTaskTable*)oRow->parent();
    CTaskFrame* pTskFrame =(CTaskFrame*)pThis->parent()->parent();
    CMainFrame* pMainFrame = (CMainFrame*)pTskFrame->parent()->parent();    

    if(pMainFrame->pTskAddFrame)
        delete pMainFrame->pTskAddFrame;
    
    pMainFrame->pTskAddFrame = new CNewTaskFrame(345, 475, "编辑任务");
    pMainFrame->pTskAddFrame->end();
    
    CNewTaskFrame*  pNewTskFrm = pMainFrame->pTskAddFrame;
    pNewTskFrm->label("新建任务");

    nsYLX::CUtil::CenterWnd(pNewTskFrm, pMainFrame);
    
    CTskTimer* pTskTimer= new CTskTimer;
	int nLine = pThis->find(oRow);    
    pThis->AddRow(pNewTskFrm, pTskTimer, nLine);
   
}//OnAddRow

void CTaskTable::OnEditRow(Fl_Widget *w, void *)
{
    CTaskRow* oRow = (CTaskRow*)w->parent()->parent()->parent();
	CTaskTable* pThis = (CTaskTable*)oRow->parent();
    CTaskFrame* pTskFrame =(CTaskFrame*)pThis->parent()->parent();
    CMainFrame* pMainFrame = (CMainFrame*)pTskFrame->parent()->parent();
    if(!pMainFrame->pTskAddFrame)
    { 
       pMainFrame->pTskAddFrame = new CNewTaskFrame(345, 475, "编辑任务");
       pMainFrame->pTskAddFrame->end();       
    }
       
    CNewTaskFrame*  pWnd = pMainFrame->pTskAddFrame;
    pWnd->label("编辑任务");

    nsYLX::CUtil::CenterWnd(pWnd, pMainFrame);

    //reset  ctrl's content of pTskAddFrame
    pWnd->BuildTskEditWnd(oRow->m_pTskTimer);
    pWnd->show();
    
    while(Fl::modal() == pWnd)
    {
        Fl::wait();
    }
    
    if(pWnd->m_nRslt!=0)
    {
        return;
    }
    //update ui
    oRow->UpdateUI();
}

void CTaskTable::OnDelRow(Fl_Widget *w, void *)
{
    CTaskRow* oRow = (CTaskRow*)w->parent()->parent()->parent();
	CTaskTable* pThis = (CTaskTable*)oRow->parent();
    CTaskFrame* pTskFrame =(CTaskFrame*)pThis->parent()->parent(); 
	int nLine = pThis->find(oRow);
    const int nLineSpace = 5;

	pThis->remove(oRow);
	pThis->vtRows.erase(nLine);
#if 0
	if(oRow->m_pTskTimer)
	{
		int nTskId = oRow->m_pTskTimer->m_nId;
		pThis->m_pTskTimerMgr->DelTskTimerById(nTskId);
	}
#endif
    
	CTaskRow* oNext =0;
	for(int i = nLine,nCount= pThis->vtRows.size();i<nCount;i++)
	{
		oNext = (CTaskRow*)pThis->child(i);
		oNext->position(oNext->x(),oNext->y()-oRow->h()-nLineSpace);
	}

    pThis->m_nRowsHeightTotal -= oRow->h()+nLineSpace;
	pTskFrame->AdjustTskTableHeight();
}

