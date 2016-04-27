
#include "newtask_frame.h"
#include "Fl_Drable_Window.h"
#include <FL/Fl_Radio_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Native_File_Chooser.H>

# include <stdio.h>
# include <stdlib.h>

#ifdef WIN32
#include <stdlib.h>	// atoi
#endif /*WIN32*/

#include "newtask_frame_cb.cpp"
//extern void startdate_cb(Fl_Widget* wid, void *data);
//extern void doscheme(Fl_Choice *c, void *);

void CNewTaskFrame::Init()
{
	dragbox *box = new dragbox(0,0, w(), h());
	box->box(FL_NO_BOX);

	int px =1, py =1;
	//start date     
	Fl_Group* pGrp = new Fl_Group(px,py,w()-2*px,70,"",1);
	pGrp->color(3);
	Fl_Box* pBox = new Fl_Box(px,py,150, 20,"开始日期:");
	pBox->labelcolor(4);
	pBox->labeltype(FL_NORMAL_LABEL);
	pBox->box(FL_FLAT_BOX);
	pBox->align(FL_ALIGN_CLIP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

	py = pBox->y() + pBox->h() +2;
	Fl_Group* btnsGrp = new Fl_Group(px,py, w(), 20,"",0);

	Fl_Radio_Round_Button* pBtn = new Fl_Radio_Round_Button(px,py, 150, 20, "当前");pBtn->set();pBtn->callback(startdate_cb, pBtn); 
	Fl_Radio_Round_Button* pBtn2 = new Fl_Radio_Round_Button(pBtn->x()+pBtn->w()+5, py, 150, 20, "阳历");pBtn2->callback(startdate_cb, pBtn2);
	btnsGrp->end();

	py = btnsGrp->y() + btnsGrp->h() +2;

	char s[10]; 
	int xTmp=px; 
	Fl_Choice *choice = new Fl_Choice(px, py, 60, 25);
	choice->labelfont(FL_HELVETICA_BOLD);
	for(int i=1900;i<2051;i++)
	{ 
	 snprintf(s, 10, "%d", i); 
	 choice->add(s);
	} 
	choice->callback((Fl_Callback *)doscheme);
	px+= choice->w()+2; pBox = new Fl_Box(px,py,40, 30,"年");pBox->box(FL_FLAT_BOX);
	//month
	px += pBox->w();
	choice = new Fl_Choice(px, py, 50, 25);
	choice->labelfont(FL_HELVETICA_BOLD);
	for(int i=1;i<13;i++)
	{ 
	 snprintf(s, 10, "%d", i); 
	 choice->add(s);
	}
	choice->callback((Fl_Callback *)doscheme);
	px+= choice->w(); pBox = new Fl_Box(px,py,40, 30,"月");pBox->box(FL_FLAT_BOX);
	//day
	px += pBox->w();
	choice = new Fl_Choice(px, py, 50, 25);
	choice->labelfont(FL_HELVETICA_BOLD);
	for(int i=1;i<31;i++)
	{ 
	 snprintf(s, 10, "%d", i); 
	 choice->add(s);
	}
	choice->callback((Fl_Callback *)doscheme);
	px+= choice->w(); pBox = new Fl_Box(px,py,40, 30,"日");pBox->box(FL_FLAT_BOX);
	pGrp->end();

	Fl_Radio_Round_Button* pCurrTimeBtn = pBtn;

	//date offset 
	px =  1;
	py = pBox->y()+pBox->h()+2;
	pGrp = new Fl_Group(px,py,w()-2*px,78,"",1);
	pGrp->color(4);

	pBox = new Fl_Box(px,py,150, 20,"时间偏移:");
	pBox->labelcolor(4);
	pBox->labeltype(FL_NORMAL_LABEL);
	pBox->box(FL_FLAT_BOX);
	pBox->align(FL_ALIGN_CLIP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

	py += pBox->h()+2;
	Fl_Input* pIpt = (Fl_Input*)new Fl_Int_Input(px,py,50,25);pBox->box(FL_FLAT_BOX); px+= pIpt->w()+2; pBox = new Fl_Box(px,py,40, 30,"时");pBox->box(FL_FLAT_BOX);
	px += pBox->w()+2;
	pIpt = (Fl_Input*)new Fl_Int_Input(px,py,30,25); px+= pIpt->w()+2; pBox = new Fl_Box(px,py,40, 30,"分");pBox->box(FL_FLAT_BOX);
	px += pBox->w()+2;
	pIpt = (Fl_Input*)new Fl_Int_Input(px,py,30,25); px+= pIpt->w()+2; pBox = new Fl_Box(px,py,40, 30,"秒");pBox->box(FL_FLAT_BOX);
	px += pBox->w()+2;
	btnsGrp = new Fl_Group(px,py, 120, 25,"",0);

	pBtn = new Fl_Radio_Round_Button(px,py, 50, 20, "之前");
	pBtn2= new Fl_Radio_Round_Button(pBtn->x()+pBtn->w()+5, py, 50, 20, "之后");pBtn->set();
	btnsGrp->end();

	px = 1;
	py += btnsGrp->h()+2;
	pIpt = (Fl_Input*)new Fl_Int_Input(px,py,50,25);pBox->box(FL_FLAT_BOX); px+= pIpt->w()+2; pBox = new Fl_Box(px,py,40, 30,"天");pBox->box(FL_FLAT_BOX);
	px += pBox->w()+2;
	pIpt = (Fl_Input*)new Fl_Int_Input(px,py,50,25);pBox->box(FL_FLAT_BOX); px+= pIpt->w()+2; pBox = new Fl_Box(px,py,40, 30,"时");pBox->box(FL_FLAT_BOX);
	px += pBox->w()+2;
	pIpt = (Fl_Input*)new Fl_Int_Input(px,py,30,25); px+= pIpt->w()+2; pBox = new Fl_Box(px,py,40, 30,"分");pBox->box(FL_FLAT_BOX);
	px += pBox->w()+2;
	pIpt = (Fl_Input*)new Fl_Int_Input(px,py,30,25); px+= pIpt->w()+2; pBox = new Fl_Box(px,py,40, 30,"秒");pBox->box(FL_FLAT_BOX);
	pGrp->end();

	//frequency 
	px = 1;
	py +=  pBox->h()+2; 
	pGrp = new Fl_Group(px,py, w(), 125,"",1);
	pGrp->color(5);
	py+= 2;
	pBox = new Fl_Box(px,py,150, 20,"频率:");
	pBox->labelcolor(4);
	pBox->labeltype(FL_NORMAL_LABEL);
	pBox->box(FL_FLAT_BOX);
	pBox->align(FL_ALIGN_CLIP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

	py += pBox->h();
	pBox = new Fl_Box(px,py,80, 20,"共执行:");
	pBox->labeltype(FL_NORMAL_LABEL);
	pBox->box(FL_FLAT_BOX);
	pBox->align(FL_ALIGN_CLIP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE); 
	px += pBox->w()+1;
	pIpt = (Fl_Input*)new Fl_Int_Input(px,py, 60,25);
	px += pIpt->w()+1;
	pBox = new Fl_Box(px,py,180,20," 次(-1表示无限次)");
	pBox->labeltype(FL_NORMAL_LABEL);
	pBox->box(FL_FLAT_BOX);
	pBox->align(FL_ALIGN_CLIP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

	py += pBox->h()+1;
	px = 1; 
	pBox = new Fl_Box(px,py,150, 20,"每次间隔:");
	pBox->labeltype(FL_NORMAL_LABEL);
	pBox->box(FL_FLAT_BOX);
	pBox->align(FL_ALIGN_CLIP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE); 
	py += pBox->h()+1;
	btnsGrp = new Fl_Group(px,py, w(), 60,"",1);
	btnsGrp->color(2);
	pBtn = new Fl_Radio_Round_Button(px,py, 50, 20);pBtn->set();
	px += pBtn->w()+2;
	pIpt = (Fl_Input*)new Fl_Int_Input(px,py,50,25);pBox->box(FL_FLAT_BOX); px+= pIpt->w()+2; pBox = new Fl_Box(px,py,40, 30,"月");pBox->box(FL_FLAT_BOX);

	px=1;
	py += pBox->h()+2;
	pBtn2 = new Fl_Radio_Round_Button(px, py, 50, 20);
	px += pBtn->w()+2;
	pIpt = (Fl_Input*)new Fl_Int_Input(px,py,50,25);pBox->box(FL_FLAT_BOX); px+= pIpt->w()+2; pBox = new Fl_Box(px,py,40, 30,"时");pBox->box(FL_FLAT_BOX);
	px += pBox->w()+2;
	pIpt = (Fl_Input*)new Fl_Int_Input(px,py,30,25); px+= pIpt->w()+2; pBox = new Fl_Box(px,py,40, 30,"分");pBox->box(FL_FLAT_BOX);
	px += pBox->w()+2;
	pIpt = (Fl_Input*)new Fl_Int_Input(px,py,30,25); px+= pIpt->w()+2; pBox = new Fl_Box(px,py,40, 30,"秒");pBox->box(FL_FLAT_BOX);

	btnsGrp->end();
	pGrp->end();
	//tsk names
	px=1;py = pGrp->y()+pGrp->h()+2;
	pBox = new Fl_Box(px,py,70, 20,"任务名称:");
	pBox->labelcolor(4);
	pBox->labeltype(FL_NORMAL_LABEL);
	pBox->box(FL_FLAT_BOX);
	pBox->align(FL_ALIGN_CLIP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	px += pBox->w()+2;
	pIpt = new Fl_Input(px,py,w()-px-2,25);

	//actions
	px=1;py += pIpt->h()+2;
	pGrp = new Fl_Group(px,py, w(), 125,"",1);
	pGrp->color(3);
	py+= 2;
	pBox = new Fl_Box(px,py,70, 20,"执行动作:"); 
	pBox->labelcolor(4);
	pBox->labeltype(FL_NORMAL_LABEL);
	pBox->box(FL_FLAT_BOX);
	pBox->align(FL_ALIGN_CLIP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	px+= pBox->w()+2;
	choice = new Fl_Choice(px, py, 120, 25);
	choice->labelfont(FL_HELVETICA_BOLD);
	const char* artskName[]={
	 "指定应用",
	 "关机","注销","重启"
	};
	for(int i=0,n=sizeof(artskName)/sizeof(artskName[0]);i<n;i++)
	{   
		choice->add(artskName[i]);
	}//for 
	 
	choice->callback((Fl_Callback *)actionChoice);
	choice->value(0);
	px=1;
	py+= choice->h()+2;
	 
	btnsGrp = new Fl_Group(px,py, w(), 70,"",1);
	btnsGrp->color(8);
	pBox = new Fl_Box(px,py,40, 20,"文件:");px+=pBox->w()+1;
	pIpt = new Fl_Input(px,py,220,25);px+=pIpt->w()+1;
	Fl_Button* pNrBtn = new Fl_Button(px,py,50,25,"浏览...");px+=pNrBtn->w()+1;
	pNrBtn->callback(fileChoose);
	//pNrBtn = new Fl_Button(px,py,20,25,"+");
	py+=pNrBtn->h()+2;
	px=1;
	pBox = new Fl_Box(px,py,80, 20,"命令行参数:");px+=pBox->w()+1;
	pIpt = new Fl_Input(px,py,w()-px-2,25);px+=pIpt->w()+1; 
	btnsGrp->end();
	pGrp->end();

	//ok or cancel
	px=1;py = pGrp->y()+pGrp->h()+2;
	pGrp = new Fl_Group(px,py, w(), 30,"",1);
	pGrp->color(3);
	py+= 2;
	px = w()-120; 
	pNrBtn = new Fl_Button(px,py,40,25,"取消");pNrBtn->callback(ok_cb, 0); px+= pNrBtn->w()+30;
	pNrBtn = new Fl_Button(px,py,40,25,"确定");pNrBtn->callback(ok_cb, 0); 

	pGrp->end();
	end();
	pCurrTimeBtn->do_callback((Fl_Widget*)pCurrTimeBtn,(void*)pBtn);
} 

CNewTaskFrame::CNewTaskFrame(int w, int h, const char* title):Fl_Window(w,h,title)
{
    Init();
}

CNewTaskFrame::CNewTaskFrame(int x, int y, int w, int h, const char* title):Fl_Window(x,y,w,h,title)
{
    Init();
}
