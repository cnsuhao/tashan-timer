
#pragma once
#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/glu.h>
#include <FL/glut.H> 
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Tiled_Image.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Tile.H>
#include <FL/Fl_Scroll.H>

#include "childframe.h"


#define MENUBARH 25

class CMainFrame:public Fl_Double_Window{
 private:
    void Init();
 public:
    CMainFrame(int w, int h, const char* title= 0);
    CMainFrame(int x, int y, int w, int h, const char* title = 0);
    void resize(int,int,int,int);
    void draw();
    int  handle(int event);
    void show(int a, char **b);
    void flush(){Fl_Double_Window::flush();};
    Fl_Sys_Menu_Bar *menubar_;
    Fl_Group *pgroup;
    Fl_Tile *m_tile;
    CChildFrame * childframe;
    class CNewTaskFrame* pTskAddFrame;
    //Fl_Double_Window* make_newtskwnd();
};
