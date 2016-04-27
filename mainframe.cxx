

#include"mainframe.h"
#include <FL/fl_ask.H>

#include "test/pixmaps/tile.xpm"
#include "about_panel.h"
#include "newtask_frame.h"
#include "taskframe.h"
#include <assert.h>
#include "Fl_Drable_Window.h"

// This callback is invoked whenever the user clicks an item in the menu bar
static void MyMenuCallback2(Fl_Widget *w, void *) {
  Fl_Menu_Bar *bar = (Fl_Menu_Bar*)w;				// Get the menubar widget
  const Fl_Menu_Item *item = bar->mvalue();			// Get the menu item that was picked

  char ipath[256]; bar->item_pathname(ipath, sizeof(ipath));	// Get full pathname of picked item

  fprintf(stderr, "callback: You picked '%s'", item->label());	// Print item picked
  fprintf(stderr, ", item_pathname() is '%s'", ipath);		// ..and full pathname

  if ( item->flags & (FL_MENU_RADIO|FL_MENU_TOGGLE) ) {		// Toggle or radio item?
    fprintf(stderr, ", value is %s", item->value()?"on":"off");	// Print item's value
  }
  fprintf(stderr, "\n");
  if ( strcmp(item->label(), "Google") == 0 ) { ; }
  if ( strcmp(item->label(), "&Quit") == 0 ) { exit(0); }
  

}

 
  static Fl_Menu_Item	items[] = {
#if 0
//{"新建任务",FL_COMMAND | 'n',MyMenuCallback2,0,0},
//{0},
{"设置",0,MyMenuCallback2,0,FL_SUBMENU},
{0},
      { "&Help", 0, 0, 0, FL_SUBMENU },
      {"关于",FL_COMMAND ,MyMenuCallback2,0,0},
      {0},
      {0}
#endif
#if 1
    { "&Game", 0, 0, 0, FL_SUBMENU },
    { "&New Game", FL_COMMAND | 'n', MyMenuCallback2, 0, FL_MENU_DIVIDER },
    { "&Check Game", FL_COMMAND | 'c', MyMenuCallback2, 0, 0 },
    { "&Restart Game", FL_COMMAND | 'r', MyMenuCallback2, 0, 0 },
    { "&Solve Game", FL_COMMAND | 's', MyMenuCallback2, 0, FL_MENU_DIVIDER },
    { "&Update Helpers", 0, MyMenuCallback2, 0, 0 },
    { "&Mute Sound", FL_COMMAND | 'm', MyMenuCallback2, 0, FL_MENU_TOGGLE | FL_MENU_DIVIDER },
    { "&Quit", FL_COMMAND | 'q', MyMenuCallback2, 0, 0 },
    { 0 },
    { "&Difficulty", 0, 0, 0, FL_SUBMENU },
    { "&Easy", 0, MyMenuCallback2, (void *)"0", FL_MENU_RADIO },
    { "&Medium", 0, MyMenuCallback2, (void *)"1", FL_MENU_RADIO },
    { "&Hard", 0,  MyMenuCallback2, (void *)"2", FL_MENU_RADIO },
    { "&Impossible", 0, MyMenuCallback2, (void *)"3", FL_MENU_RADIO },
    { 0 },
    { "&Help", 0, 0, 0, FL_SUBMENU },
    { "&About Sudoku", FL_F + 1, MyMenuCallback2, 0, 0 },
    { 0 },
    { 0 }
#endif
  };


void about_cb(Fl_Widget *o, void * param)
{
  CMainFrame* pThis = (CMainFrame*)(o->parent());

  float nScreenW = pThis->w();
  float nScreenH = pThis->h();
  //printf("w:%f, h:%f\n",nScreenW,nScreenH);
  
  const  char* s = (char*)param;
  Fl_Widget*  pWnd = 0;
  if(strcmp(s,"newtsk")==0)
  {
      if(!pThis->pTskAddFrame)
      { 
         pThis->pTskAddFrame = new CNewTaskFrame(345, 475, "新建定时任务");
      }
      pWnd = pThis->pTskAddFrame;
  }else if(strcmp(s,"about")==0)
  {
      if (!about_panel) make_about_panel();
      pWnd = about_panel;
  }
  if(!pWnd) return;
  int winW=pWnd->w(),winH=pWnd->h();
  int dx = (nScreenW-winW)/2 + pThis->x();
  int dy = (nScreenH-winH)/2 + pThis->y();
  pWnd->position(dx,dy);
  pWnd->show();
}

void cb(Fl_Widget *w, void *) { 
    //static char msg[200];
    //snprintf(msg, sizeof(msg)-1,"you have press %s", w->label());
    //msg[sizeof(msg)-1]=0;
    fl_alert("you have press %s", w->label());
}
 
void CMainFrame::Init()
{
    pTskAddFrame=0;
	const int dimW = w();//751;//476;
	const int dimH = h();//642;//444;
	const int menubarH = MENUBARH;
	menubar_ = new Fl_Sys_Menu_Bar(0, 0, w(), MENUBARH);
    menubar_->add("新建任务",0,about_cb,(void*)"newtsk",0);
	menubar_->add("设置",0,MyMenuCallback2);
	menubar_->add("帮助",0,0,0,FL_SUBMENU);  
    menubar_->add("帮助/关于",FL_F+1,about_cb,(void*)"about",0);

    //menubar_->image(new Fl_Tiled_Image(new Fl_Pixmap((const char * const *)tile_xpm)));

    //childframe = new CChildFrame(0, MENUBARH,w(),h()-MENUBARH);
    //childframe->box(FL_NO_BOX);
    //childframe->resizable(*childframe);
  
    dragbox *box = new dragbox(0,menubarH, dimW, dimH-menubarH);
    //box->image(new Fl_Tiled_Image(new Fl_Pixmap((const char * const *)tile_xpm)));
    //box->label("xxx");
    box->box(FL_FLAT_BOX);

    Fl_Group *g = new Fl_Group(0,menubarH, dimW, dimH-menubarH);
    g->box(FL_NO_BOX);
    g->end();
    
	int dx = 0, dy = dx; // border width of resizable() - see below
	int yOffset = MENUBARH;
	Fl_Tile *tile=new Fl_Tile(0,yOffset, w(), h()-MENUBARH);

	Fl_Window *box0=new Fl_Window(0,yOffset,150,h()-yOffset,"0");
	box0->box(FL_DOWN_BOX);
	box0->color(9);
	box0->labelsize(36);
	//box0->align(FL_ALIGN_CLIP);
    box0->size_range(150, box0->h(), 0, 0);
    size_range(150,1,0,0);//611 459

	Fl_Group *box0_g=new Fl_Group(25, 28, box0->w(), 20);
	box0_g->box(FL_NO_BOX);

	struct
	{
	  int dy;
	  const char* name;
	  Fl_Callback_p pfn;
	} arBtns[] ={
	  {0,  "关机...", cb},
	  {35, "锁屏...", cb},
	  {35, "待机...", cb},
	  {35, "休眠...", cb},
	  {35, "护眼...", cb},
	};

	for(int i=0,ncount=sizeof(arBtns)/sizeof(arBtns[0]),dy=28;i<ncount;i++)
	{
	  dy += arBtns[i].dy;
	  Fl_Button* b=new Fl_Button(25, dy, 80, 20, arBtns[i].name);
	  b->image(new Fl_Tiled_Image(new Fl_Pixmap((const char * const *)tile_xpm)));
	  b->align(FL_ALIGN_CLIP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
	  b->callback(arBtns[i].pfn);
	  
	  box0_g->end();
	}//for

	box0_g->end();
	box0->end();  
	box0_g->resizable(*g);
	dx=dy=0;
	//Fl_Box *r=new Fl_Box(tile->x()+dx+150,tile->y()+dy,tile->w()-2*dx-150,tile->h()-2*dy);
	//tile->resizable(*r);
	// r.box(FL_BORDER_FRAME);

    //r=new Fl_Box(150,MENUBARH,w()-150,h()-MENUBARH);//(tskfrm->x(),tskfrm->y(), tskfrm->w(), tskfrm->h());
    //r->box(FL_BORDER_FRAME);
    //Fl_Scroll *scr=new Fl_Scroll(150,MENUBARH,w()-150,h()-MENUBARH);
    //scr->begin();
	CTaskFrame *tskfrm=new CTaskFrame(150,MENUBARH,w()-150,h()-MENUBARH,"1");

    
	tskfrm->box(FL_BORDER_FRAME);
	tskfrm->size_range(350,100,0,0);
    

    //tskfrm->resizable(scr);
    tskfrm->end();
    //scr->end();    

    //scr->resizable(*tskfrm);
	//tile->resizable(*tskfrm);
    //tskfrm->resizable(*tskfrm);
	tile->end();
	end();

    Fl_Box *r=new Fl_Box(tile->x()+dx+150,tile->y()+dy,tile->w()-2*dx-150,tile->h()-2*dy);
	tile->resizable(*r);
    
	resizable(*g);

}

CMainFrame::CMainFrame(int w, int h, const char* title):Fl_Double_Window(w,h,title)
{
    Init();
}

CMainFrame::CMainFrame(int x, int y, int w, int h, const char* title ):Fl_Double_Window(x,y,w,h,title)
{
    Init();
}

void CMainFrame::show(int argc, char **argv)
{
 

  Fl_Double_Window::show(argc,argv);
}

int CMainFrame::handle(int event)
{

    return Fl_Double_Window::handle(event);
}

void CMainFrame::resize(int X,int Y,int W,int H) {

    Fl_Double_Window::resize( X, Y, W, H);
}

void CMainFrame::draw()
{

    Fl_Double_Window::draw();
}
