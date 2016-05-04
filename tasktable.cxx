

#include "tsktimer/TskTimer.h"
#include "tsktimer/TskTimerMgr.h"
#include "taskrow.h"
#include "tasktable.h"

CTaskTable::CTaskTable(int X, int Y, int W, int H, const char* L) : Fl_Group(X,Y,W,H,L)
{
    m_nRowsHeightTotal=0;
    fl_color(3);

    m_pTskTimerMgr = new CTskTimerMgr();
    m_pTskTimerMgr->LoadTskXml("task.xml");

    int yOffset=1;
    int nLineSpace = 5;
	//根据m_pTskTimerMgr里的数据，可以设定和启动相应的计时器鸟。
	CListHead* plistEntry = &m_pTskTimerMgr->m_pTskTimerEntry->m_ListEntry;
    CTaskRow* oRow=0;
	for(CListHead* pList = plistEntry->prev; pList != plistEntry ;pList = pList->prev)
	{
		CTskTimer* p = CONTAINING_RECORD(pList, CTskTimer, m_ListEntry);
        oRow = BuildTskRow(yOffset, p);
        yOffset+=nLineSpace;
        vtRows.push_back((long)oRow);
	}//for
    if(oRow) m_nRowsHeightTotal = oRow->y() + oRow->h();
	end();

    begin();
	Fl_Group *gp3 = new Fl_Group(0, yOffset, w()-0, 35, 0,0);
    resizable(gp3);
    gp3->box(FL_NO_BOX);
    BuildTskRow(yOffset, 0);
    gp3->end();
    gp3->hide();
	end();

	m_nRowsHeightTotal+= 40;
	begin();
}

CTaskTable::~CTaskTable()
{
    if(rowImg)
    {
        delete rowImg;
        rowImg=0L;
    }
    if(m_pTskTimerMgr) delete m_pTskTimerMgr;

} 

CBtnStruc btnsStruc[]={
        {{2,18},"增加任务",&CTaskTable::OnAddRow},
        {{2+18,18},"增加子任务",&CTaskTable::OnAddRow},        
        {{2+18*2,18},"上移",&CTaskTable::OnMoveUpDown},
        {{2+18*3,18},"下移",&CTaskTable::OnMoveUpDown},
        {{2+18*4,18},"暂停",&CTaskTable::OnTskPause},
        {{2+18*6,18},"编辑",&CTaskTable::OnEditRow},
        {{2+18*7,18},"删除",&CTaskTable::OnDelRow},
        {{2+18*5,18},"启动",&CTaskTable::OnTskPause},            
};

int CTaskTable::GetSelRow()
{
    int i=0,nCount = children()-1;
    CTaskRow* pRow=0;
    for(;i<nCount;i++)
    {
        pRow = (CTaskRow*)child(i);
        if(!pRow || pRow->m_bLeftBtnDown) break;
    }

    if(nCount == 0) i=-1;
    
    return i;
}

void CTaskTable::draw()
{
    fl_color(FL_BACKGROUND2_COLOR);
    fl_rectf(x(),y(),w(),h());

    Fl_Group::draw();
}

