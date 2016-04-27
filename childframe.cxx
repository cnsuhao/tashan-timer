
#include"childframe.h"

CChildFrame::CChildFrame(int w, int h, const char* title):Fl_Window(w,h,title)
{

}

CChildFrame::CChildFrame(int x, int y, int w, int h, const char* title ):Fl_Window(x,y,w,h,title)
{

}

int CChildFrame::handle(int event)
{

    return Fl_Window::handle(event);
}

void CChildFrame::draw()
{

    Fl_Window::draw();
}
