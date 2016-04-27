

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
#include <FL/Fl_Browser.H>
#include <FL/fl_ask.H>

#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Input.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Table.H>
#include "utils/intvec.h"
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Image.H>

 
class CNewTaskFrame:public Fl_Window{
public:
    CNewTaskFrame(int w, int h, const char* title= 0);
    CNewTaskFrame(int x, int y, int w, int h, const char* title = 0);
    void Init();
    //void draw();
    //int  handle(int event);

};
