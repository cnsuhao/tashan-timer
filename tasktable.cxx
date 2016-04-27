
#include "taskrow.h"
#include "tasktable.h"

CTaskTable::CTaskTable(int x, int y, int w, int h, const char *l) : Fl_Group(x,y,w,h,l)
{
    nDims=2;
    fl_color(3);
}

CTaskTable::~CTaskTable()
{
    if(rowImg)
    {
        delete rowImg;
        rowImg=0L;
    }
} 

CBtnStruc btnsStruc[]={
        {{2+18,18},"增加任务",&CTaskTable::OnAddRow},
        {{2,18},"增加子任务",&CTaskTable::OnAddRow},        
        {{2+18*2,18},"上移",&CTaskTable::OnAddRow},
        {{2+18*3,18},"下移",&CTaskTable::OnAddRow},
        {{2+18*4,18},"暂停",&CTaskTable::OnAddRow},
        {{2+18*6,18},"编辑",&CTaskTable::OnAddRow},
        {{2+18*7,18},"删除",&CTaskTable::OnDelRow}      
};

void CTaskTable::SetSize(int newrows)
{
    int yOffset =0;
    
    begin();
    CTaskRow* oRow =  0;
    for(int r=0;r<newrows;r++)
    {
        oRow =  new CTaskRow(0, yOffset,w(), 35,"",0);
        yOffset=oRow->BuildRow(1, yOffset+5, 35, btnsStruc);
        oRow->end();
        vtRows.push_back((long)oRow);      
    }
	end();
    
    yOffset += 14;
    
    begin();
    Fl_Group *group3 = new Fl_Group(0, yOffset, w()-0, 35, 0,1);
    resizable(group3);
    group3->end();
    end();
}//SetSize

void CTaskTable::draw()
{
    fl_color(FL_BACKGROUND2_COLOR);
    fl_rectf(x(),y(),w(),h());

    Fl_Group::draw();
}

