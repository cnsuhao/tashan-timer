
#include "taskrow.h"
#include "ui/imgdata.h"
#include "utils/utils.h"
#include "tasktable.h"

XPoint CTaskRow::unitWHs[]={
    {28,28},//目录树展开收缩图标
    {187,31},//任务名称
    {190,31},//剩余时间
    {28,28},//增加任务图标
    {28,28},//增加子任务图标
    {28,28},//暂停图标
    {28,28},//编辑图标
    {28,28},//上移图标
    {28,28},//下移图标
    {28,28}//删除图标     
};

XPoint CTaskRow::unitSpaces[]={
    {2,0},
    {2,0},
    {2,0},//增加任务图标
    {2,0},//增加子任务图标
    {2,0},//暂停图标
    {2,0},//编辑图标
    {2,0},//上移图标
    {2,0},//下移图标
    {2,0},//删除图标
    {2,0}    
};
int unitWHCount = sizeof(CTaskRow::unitWHs)/sizeof(CTaskRow::unitWHs[0]);

CTaskRow::CTaskRow(int x, int y, int w, int h, const char *l,bool b) : Fl_Group(x,y,w,h,l,b)
{
    m_bMouseEnter = false;
    m_pTskTimer = 0;
    m_icons=0;
}

int CTaskRow::BuildRow(const int pX, const int pY, const int nLineH, CBtnStruc* btnsStruc)
{
	int X=pX, Y=pY,H=nLineH;
    int hOffset = 0;
	begin();
	//at first I set width to max, at last I readjust row width
	Fl_Group * group1 = new Fl_Group(X, Y, w()-2*X, H-hOffset, "",0);

	//目录树展开收缩图标
	Fl_Button *butt = new Fl_Button(X, Y,H-2,H-hOffset,"-");
	butt->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
	butt->color(12,14);
    butt->tooltip("展开");

	//任务名称
	X += butt->w() +  CTaskRow::unitSpaces[0].x;
	Fl_Box* tskTitle = new Fl_Box(X, Y, CTaskRow::unitWHs[1].x, CTaskRow::unitWHs[1].y,"");
	tskTitle->box(FL_BORDER_FRAME);
	tskTitle->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_CLIP);
	tskTitle->position(tskTitle->x(), Y+(butt->h() - tskTitle->h())/2);

	//剩余时间
	X += tskTitle->w() +  CTaskRow::unitSpaces[1].x;
	Fl_Box* tskLeft = new Fl_Box(X, Y, CTaskRow::unitWHs[2].x, CTaskRow::unitWHs[2].y,"剩余 01年05月32天 03:20:95");
	tskLeft->box(FL_BORDER_FRAME);
	tskLeft->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	tskLeft->position(tskLeft->x(), Y+(butt->h() - tskLeft->h())/2);

    if(!m_icons)
	   m_icons = (Fl_Image*)new Fl_RGB_Image(edit_png,144,36,4);
    
	Fl_Image* acIcon = 0;
	X += tskLeft->w() +  CTaskRow::unitSpaces[2].x;

	int nIconWidths = 0;
	for(int i=3;i<10;i++)
	{
		nIconWidths +=  CTaskRow::unitWHs[i].x  +  CTaskRow::unitSpaces[i].x;
	}

    Y+= 2;
	Fl_Group* pIconsGroup = new Fl_Group(X, Y, nIconWidths,33-hOffset-2,"",1);
	for(int i=3,j=0;i<10;i++,j++)
	{
		butt = new Fl_Button(X+1, Y+3,CTaskRow::unitWHs[i].x, CTaskRow::unitWHs[i].y);
		butt->box(FL_NO_BOX);
		butt->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);

		acIcon = CUtil::copy(m_icons,btnsStruc[j].pt.x,btnsStruc[j].pt.y,18,18);
        
		butt->image(acIcon);
		butt->tooltip(btnsStruc[j].tips);
		butt->callback(btnsStruc[j].pfn);
		X += butt->w() +  CTaskRow::unitSpaces[i].x;
	}//for
	pIconsGroup->end();

	group1->end();
	end();
	group1->resizable(tskTitle);
	resizable(*group1);

	group1->align(FL_ALIGN_CLIP|FL_ALIGN_LEFT|FL_ALIGN_TOP);

	return group1->y()+group1->h();
}


CTaskRow::~CTaskRow()
{

}

void CTaskRow::draw()
{
    Fl_Group::draw();
    if(m_bMouseEnter)
    {
        fl_color(2);
        fl_rect(x()+1,y(),w()-2,h());
    }
}

int CTaskRow::handle(int event)
{
  switch (event)
  {
  case FL_LEAVE:
      m_bMouseEnter = false;
      redraw();
      break;
  case FL_ENTER:
      {
          m_bMouseEnter = true;
          redraw();
      }
  }
  return Fl_Group::handle(event);
}
