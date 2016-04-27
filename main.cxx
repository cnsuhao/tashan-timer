//
// "$Id: shapedwindow.cxx 10325 2014-09-21 14:10:36Z manolo $"
//
// shapedwindow example source file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2014 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     http://www.fltk.org/COPYING.php
//
// Please report all bugs and problems to:
//
//     http://www.fltk.org/str.php
//

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
#include <FL/fl_ask.H>
#include "test/pixmaps/tile.xpm"
#include "taskframe.h"
#include "mainframe.h"


#include "ui/imgdata.h" //main_png[]

int main(int argc, char **argv)
{
    const int dimW = 628+150;
    const int dimH = 642;
    const int menubarH = MENUBARH;
    int x,y,w,h;
    Fl::screen_xywh	(x,y,w,h);
    float nScreenW = w;
    float nScreenH = h;
    x = (nScreenW-dimW)/2;
    y = (nScreenH-dimH)/2;
    CMainFrame window(x,y,dimW, dimH);
    window.box(FL_NO_BOX);

    Fl_RGB_Image  icon(main_png,96,96,4); 
    window.icon(&icon);

    window.label("他山定时 v1.0.0.7");
    window.show(argc, argv);
    
    Fl::run();
    return 0;
}
