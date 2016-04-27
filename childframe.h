
#pragma once
#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/glu.h>
#include <FL/glut.H> 
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Tiled_Image.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_Gl_Window.H>

class CChildFrame:public Fl_Window{
public:
    CChildFrame(int w, int h, const char* title= 0);
    CChildFrame(int x, int y, int w, int h, const char* title = 0);
    void draw();
    int  handle(int event);

};
