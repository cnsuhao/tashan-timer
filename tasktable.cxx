
#include "taskrow.h"
#include "tasktable.h"

CTaskTable::CTaskTable(int x, int y, int w, int h, const char *l) : Fl_Group(x,y,w,h,l)
{
    nDims=2;
    fl_color(3);
//clip_children(1);
    //type(FL_WINDOW);
//box(FL_NO_BOX);
//labeltype(FL_NORMAL_LABEL);
    //fl_register_images();
 //callback((Fl_Callback*)default_callback);

}

CTaskTable::~CTaskTable()
{
    if(rowImg)
    {
        //rowImg->release();
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
    //const int newcols = nDims;
    int yOffset =0;
    
    begin();
    CTaskRow* oRow =  0;
    for(int r=0;r<newrows;r++)
    {
        oRow =  new CTaskRow(0, yOffset,w(), 35,"",0);
        yOffset=oRow->BuildRow(1, yOffset+5, 35, btnsStruc);
        oRow->end();
        vtRows.push_back((long)oRow);

        //Fl_Group *group3 = new Fl_Group(0, yOffset+14, w()-0, 35, 0,1);
        //resizable(group3);
        //group3->end();        
    }
	end();
    
    yOffset += 14;
#if 1
    begin();
    Fl_Group *group3 = new Fl_Group(0, yOffset, w()-0, 35, 0,1);
    resizable(group3);
    group3->end();
    end();
 #endif
#if 0
    int X=1,Y=1+yOffset;
    Fl_Box *b0  = new Fl_Box(X, Y, CTaskRow::unitWHs[0].x, CTaskRow::unitWHs[0].y);  //Fl_Box是默认不可见的

    X += b0->w() +  CTaskRow::unitSpaces[0].x;
    b0 = new Fl_Box(X, Y,CTaskRow::unitWHs[1].x, CTaskRow::unitWHs[1].y);


    X += b0->w() +  CTaskRow::unitSpaces[1].x;
    b0 = new Fl_Box(X, Y,CTaskRow::unitWHs[2].x, CTaskRow::unitWHs[2].y);
//add_resizable(*b0);

	for(int i=3;i<10;i++)
	{
		X +=  b0->w() +  CTaskRow::unitSpaces[i-1].x;
		b0 = new Fl_Box(X, Y,CTaskRow::unitWHs[i].x, CTaskRow::unitWHs[i].y);
	}


    group3->end();
    group3->align(FL_ALIGN_CLIP);
#endif
//Fl_Scroll *scr=new Fl_Scroll(0,0,w()-200,h());
//add_resizable(*scr);
//group3->resize(0, yOffset, w()-0, 30);
}//SetSize
 
#if 1
void CTaskTable::draw()
{
    fl_color(FL_BACKGROUND2_COLOR);
    fl_rectf(x(),y(),w(),h());
    //  draw_box();
    Fl_Group::draw();
    //Fl_Window::draw_children();
}
#endif
