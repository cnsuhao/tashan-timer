
#include "include/stdafx.h"
#include "tsktimer/TskTimer.h"
#include "tsktimer/TskTimerMgr.h"
#include "taskrow.h"
#include "tasktable.h"
#include "taskframe.h"
#include "ui/imgdata.h"
#include "utils/utils.h"
#include <FL/fl_ask.H>

extern CBtnStruc btnsStruc[];


CTaskRow* CTaskTable::BuildTskRow(int& yOffset, CTskTimer* pTskTimer)
{ 
    CTaskRow* oRow =0;

	oRow =  new CTaskRow(0, yOffset,w(), 35,"",0);
	yOffset=oRow->BuildRow(1, yOffset, 35, btnsStruc);
    oRow->m_pTskTimer = pTskTimer;
	oRow->end();
		
	//modify tsk name
	Fl_Group * gp0 = (Fl_Group*)oRow->child(0);
	Fl_Widget* tskTitle =  (Fl_Widget*)gp0->child(1);

	tskTitle->copy_label(pTskTimer->m_pTskName);

	return oRow;
}//BuildTskRow

void CTaskTable::swapRow(CTaskRow* a, CTaskRow* b)
{
	int na = find(a);
	int nb = find(b);

    if(na==nb || a==b) return;

	if(na<nb)
	{
		remove(na);
		remove(nb-1); 
		insert(*b, na);
		insert(*a, nb);

		na = a->x(),nb=a->y();
		a->position(b->x(),b->y());
		b->position(na,nb);
		vtRows.swap(na,nb);
	}else
	{
		swapRow(b,a);
		return;
	}
	redraw();
}

void CTaskTable::OnMoveUpDown(Fl_Widget *w ,void* p)
{
	CTaskRow* oRow = (CTaskRow*)w->parent()->parent()->parent();
    CTaskTable* pThis = (CTaskTable*)oRow->parent();
	int nLine = pThis->find(oRow);

    if(strcmp((const char*)w->tooltip(), btnsStruc[2].tips)==0)
	{
        //move up
		if(nLine>0)
			pThis->swapRow((CTaskRow*)pThis->child(nLine-1),oRow);
	}else
	{
	    //move down
		if(nLine<pThis->children()-2)
			pThis->swapRow((CTaskRow*)pThis->child(nLine+1),oRow);
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
		acIcon = CUtil::copy(oRow->m_icons,btnsStruc[7].pt.x, btnsStruc[7].pt.y,18,18);
		*nState &= ~PAUSE;
		*nState |= RUNNING;
	}else
	{
		acIcon = CUtil::copy(oRow->m_icons,btnsStruc[4].pt.x, btnsStruc[4].pt.y,18,18);
		*nState &= ~RUNNING;
		*nState |= PAUSE;
	}
	w->image(acIcon);
}//OnTskPause
void CTaskTable::OnAddRow(Fl_Widget *w, void *)
{
    CTaskRow* oRow = (CTaskRow*)w->parent()->parent()->parent();
	CTaskTable* pThis = (CTaskTable*)oRow->parent();
    CTaskFrame* pTskFrame =(CTaskFrame*)pThis->parent()->parent(); 
	int nLine = pThis->find(oRow);

    const int nLineSpace = 5;

//Fl_Widget* pResize = pThis->resizable();
//pThis->resizable(0);

    pThis->m_nRowsHeightTotal += oRow->h()+nLineSpace;
    pTskFrame->AdjustTskTableHeight();


    int vscrollpy = 0;//pTskFrame->getVscrollbarYpos();
	int yOffset = oRow->y() + oRow->h() + vscrollpy+nLineSpace;

	CTaskRow* oNewRow =  new CTaskRow(0, yOffset, pThis->w(), 35,"",0);
    yOffset=oNewRow->BuildRow(1, oNewRow->y(),  35, btnsStruc);
	oNewRow->end();

	//adjust row's y() between nLine+1 and lines
	CTaskRow* oNext =0;
	for(int i = nLine+1,nCount= pThis->vtRows.size();i<nCount;i++)
	{
		oNext = (CTaskRow*)pThis->child(i);
		oNext->position(oNext->x(),oNext->y()+oRow->h()+nLineSpace);
	}
	pThis->insert(*oNewRow, nLine+1);

	//adjust oNewRow's all cells's w()
	for(int i=1;i<4;i++)
	{
		Fl_Widget* oTaskNamePrev = ((Fl_Group*)oRow->child(0))->child(i);
		Fl_Widget* oNewTaskName = ((Fl_Group*)oNewRow->child(0))->child(i);
		oNewTaskName->resize(oTaskNamePrev->x(),
								 oNewTaskName->y(),
								 oTaskNamePrev->w(),
								 oNewTaskName->h());
	}

	pThis->vtRows.insert((long)oNewRow, nLine+1);

//pThis->resizable(pResize);
    pTskFrame->AdjustTskTableHeight();
//pThis->redraw();
}//OnAddRow

void CTaskTable::OnEditRow(Fl_Widget *w, void *)
{

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

	if(oRow->m_pTskTimer)
	{
		int nTskId = oRow->m_pTskTimer->m_nId;
		pThis->m_pTskTimerMgr->DelTskTimerById(nTskId);
	}

	CTaskRow* oNext =0;
	for(int i = nLine,nCount= pThis->vtRows.size();i<nCount;i++)
	{
		oNext = (CTaskRow*)pThis->child(i);
		oNext->position(oNext->x(),oNext->y()-oRow->h()-nLineSpace);
	}
//Fl_Group *rzGroup= (Fl_Group*)pThis->resizable();
//rzGroup->position(rzGroup->x(),rzGroup->y() - oRow->h()-nLineSpace);

    pThis->m_nRowsHeightTotal -= oRow->h()+nLineSpace;
pTskFrame->AdjustTskTableHeight();
//pThis->redraw();
}

