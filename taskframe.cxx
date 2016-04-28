
#include "tsktimer/TskTimer.h"
#include "tsktimer/TskTimerMgr.h"
#include "tasktable.h"
#include "taskframe.h"


static int G_usesymbols = 0;

// Global FLTK callback for drawing all label text
void MyDraw(const Fl_Label *o, int X, int Y, int W, int H, Fl_Align a) {
    fl_font(o->font, o->size);
    fl_color((Fl_Color)o->color);
    fl_draw(o->value, X, Y, W, H, a, o->image, G_usesymbols);
}

// Global FLTK callback for measuring all labels
void MyMeasure(const Fl_Label *o, int &W, int &H) {
    fl_font(o->font, o->size);
    fl_measure(o->value, W, H, G_usesymbols);
}

void CTaskFrame::BuildUI()
{
    const int dx=0, dy=0;
    Fl_Browser *b = new Fl_Browser(dx,dy,w()-dx*2,h()-dy*2);
    int widths[] = { 50, 50, 50, 70, 70, 40, 40, 70, 70, 50, 0 };               // widths for each column
    b->column_widths(widths);
    b->column_char('\t');                                                       // tabs as column delimiters
    b->type(FL_MULTI_BROWSER);
    b->add("\tUSER\tPID\t%CPU\t%MEM\tVSZ\tRSS\tTTY\tSTAT\tSTART\tTIME\tCOMMAND"); // lines of tab delimited data
    b->add("\troot\t2888\t0.0\t0.0\t1352\t0\ttty3\tSW\tAug15\t0:00\t@b@f/sbin/mingetty tty3");
    b->add("\terco\t2889\t0.0\t13.0\t221352\t0\ttty3\tR\tAug15\t1:34\t@b@f/usr/local/bin/render a35 0004");
    b->add("\tuucp\t2892\t0.0\t0.0\t1352\t0\tttyS0\tSW\tAug15\t0:00\t@b@f/sbin/agetty -h 19200 ttyS0 vt100");
    b->add("\troot\t13115\t0.0\t0.0\t1352\t0\ttty2\tSW\tAug30\t0:00\t@b@f/sbin/mingetty tty2");
    b->add("\troot\t13464\t0.0\t0.0\t1352\t0\ttty1\tSW\tAug30\t0:00\t@b@f/sbin/mingetty tty1 --noclear");
    b->align(FL_ALIGN_CLIP|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);

    b->resize(dx,dy,w()-dx*2,h()-dy*2);
    Fl::set_labeltype(FL_NORMAL_LABEL, MyDraw, MyMeasure);
    b->redraw(); 
}

void CTaskFrame::Init()
{
    m_pTskTable=new CTaskTable(0, 0, w(), h()-0, "FLTK widget table");
    m_pTskTable->end();

	resizable(m_pTskTable);
}

CTaskFrame::CTaskFrame(int w, int h, const char* title):Fl_Window(w,h,title)
{
    Init();
}

CTaskFrame::CTaskFrame(int x, int y, int w, int h, const char* title ):Fl_Window(x,y,w,h,title)
{
    Init();
}

int CTaskFrame::handle(int event)
{

    return Fl_Window::handle(event);
}

void CTaskFrame::draw()
{ 
    Fl_Window::draw();
}

