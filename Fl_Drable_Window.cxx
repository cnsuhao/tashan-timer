
#include "Fl_Drable_Window.h"


void Fl_Drable_Window::Init()
{
    //dragbox *box = new dragbox(0,menubarH, dimW, dimH-menubarH);
    //box->image(new Fl_Tiled_Image(new Fl_Pixmap((const char * const *)tile_xpm)));
    //box->label("xxx");
    //box->box(FL_FLAT_BOX);
}

Fl_Drable_Window::Fl_Drable_Window(int w, int h, const char* title):Fl_Double_Window(w,h,title)
{

}


Fl_Drable_Window::Fl_Drable_Window(int x, int y, int w, int h, const char* title ):Fl_Double_Window(x,y,w,h,title)
{
}

Fl_Drable_Window::~Fl_Drable_Window()
{

}
