
#include "tsktimer/TskTimer.h"
#include "tsktimer/TskTimerMgr.h"
#include "taskrow.h"
#include "tasktable.h"
#include "ui/imgdata.h"
#include "utils/utils.h"
#include <FL/fl_ask.H>

extern CBtnStruc btnsStruc[];


CTaskRow* CTaskTable::BuildTskRow(int& yOffset, CTskTimer* pTskTimer)
{ 
    CTaskRow* oRow =0;

	oRow =  new CTaskRow(0, yOffset,w(), 35,"",0);
	yOffset=oRow->BuildRow(1, yOffset+5, 35, btnsStruc);
	oRow->end();
		
	//modify tsk name
	Fl_Group * gp0 = (Fl_Group*)oRow->child(0);
	Fl_Widget* tskTitle =  (Fl_Widget*)gp0->child(1);

	tskTitle->copy_label(pTskTimer->m_pTskName);

	return oRow;
}//BuildTskRow

void CTaskTable::OnAddRow(Fl_Widget *w, void *)
{
    CTaskRow* oRow = (CTaskRow*)w->parent()->parent()->parent();
	CTaskTable* pThis = (CTaskTable*)oRow->parent();
	int nLine = pThis->find(oRow);

	int yOffset = oRow->y() + oRow->h();
	pThis->begin();
		CTaskRow* oNewRow =  new CTaskRow(0, yOffset, pThis->w(), 35,"",0);
        yOffset=oNewRow->BuildRow(1, yOffset+5,  35, btnsStruc);
		oNewRow->end();

        //move 
		Fl_Widget* oTaskLeftTimerPrev = ((Fl_Group*)oRow->child(0))->child(2);
		Fl_Widget* oTaskLeftTimer = ((Fl_Group*)oNewRow->child(0))->child(2);
		oTaskLeftTimer->resize(oTaskLeftTimer->x(),
				   oTaskLeftTimer->y(),
				   oTaskLeftTimerPrev->w(),
				   oTaskLeftTimer->h());

	pThis->end();

	pThis->insert(*oNewRow, nLine+1);
	pThis->vtRows.push_back((long)oNewRow);

	Fl_Group *rzGroup= (Fl_Group*)pThis->resizable();
	rzGroup->resize(rzGroup->x(),rzGroup->y()+oNewRow->h(),rzGroup->w(),rzGroup->h());

	pThis->redraw();

	fl_alert("OnAddRow %d", nLine);
}//OnAddRow

void CTaskTable::OnDelRow(Fl_Widget *w, void *)
{
    CTaskRow* oRow = (CTaskRow*)w->parent()->parent()->parent();
	CTaskTable* pThis = (CTaskTable*)oRow->parent();
	int nLine = pThis->find(oRow);

	int yOffset = oRow->y() + oRow->h();

	pThis->remove(oRow);
	pThis->vtRows.erase(nLine);
	pThis->redraw();

	fl_alert("OnDelRow %d", nLine);
}

