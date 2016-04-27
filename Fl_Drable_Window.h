
#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>


class dragbox : public Fl_Box {
public:
  dragbox(int x, int y, int w, int h, const char *t=0) : Fl_Box(x,y,w,h,t) {};
  int handle(int event) {
    static int fromx, fromy, winx, winy;
    if (event == FL_PUSH) {
      fromx = Fl::event_x_root();
      fromy = Fl::event_y_root();
      winx = window()->x_root();
      winy = window()->y_root();
      return 1;
    }
    else if (event == FL_DRAG) {
      int deltax = Fl::event_x_root() - fromx;
      int deltay = Fl::event_y_root() - fromy;
      window()->resize(winx + deltax, winy + deltay, window()->w(), window()->h());
      return 1;
    }
    return Fl_Box::handle(event);
  }
    void draw()
    {
        return Fl_Box::draw();
    }
};

class Fl_Drable_Window:public Fl_Double_Window
{
 public:
    Fl_Drable_Window(int w, int h, const char* title= 0);
    Fl_Drable_Window(int x, int y, int w, int h, const char* title = 0);    
    void Init();
    ~Fl_Drable_Window();
};
