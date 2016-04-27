
#pragma once
#include <config.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Radio_Light_Button.H>
#include <FL/Fl_Slider.H>
#include <stdlib.h>

#if !HAVE_GL
class cube_box : public Fl_Box {
public:
  double lasttime;
  int wire;
  double size;
  double speed;
  cube_box(int x,int y,int w,int h,const char *l=0)
    :Fl_Box(FL_DOWN_BOX,x,y,w,h,l){
      label("This demo does\nnot work without GL");
  }
};
#else
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

class cube_box : public Fl_Gl_Window {
  void draw();
  int handle(int);
  void InitializeGL();
  void DrawCube();
public:
  double lasttime;
  int wire;
  double size;
  double speed;
  cube_box(int x,int y,int w,int h,const char *l=0);
  void draw_overlay();
  virtual ~cube_box();
  void init();
};



#define v3f(x) glVertex3fv(x)

#endif
