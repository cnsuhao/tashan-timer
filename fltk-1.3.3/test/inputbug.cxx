


#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_Input.H>

class CMainFrame:public Fl_Group
{
public:
	CMainFrame(int x, int y, int w, int h, const char* title = 0): Fl_Group(x,y,w,h,title)
	{
      box(FL_ENGRAVED_BOX); 
      color(FL_BACKGROUND2_COLOR); 
	  Fl_Input *ipt=new Fl_Input(x+10,y+10,200,20);
	  //ipt=new Fl_Input(x+10,y+10+32,200,20);
	}
#if 1
	void draw()
	{
		fl_color(FL_BACKGROUND2_COLOR);
		fl_rectf(x(),y(),w(),h());
		Fl_Group::draw();
	}
    #endif
};

int main(int argc,char** argv)
{
    Fl_Window wnd(0,0,600,500);
    CMainFrame cf(10,10,400,400);
    wnd.show(argc, argv);

    Fl::run();
    return 0;
}
