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


Fl_RGB_Image* prepare_shape(int x, int y, int w, int h)
{
  // draw a white circle with a hole in it on black background
  Fl_Image_Surface *surf = new Fl_Image_Surface(w, h);
  Fl_Surface_Device* current = Fl_Surface_Device::surface();
  //if(!surf) return 0;

  surf->set_current();

  fl_color(FL_BLACK);
  fl_rectf(-1+x, -1+y, w+2, h+2);
  //fl_color(FL_WHITE);
  //fl_pie(2,2,w-4,h-4,0,360);
  //fl_color(FL_BLACK);
  //fl_pie(0.7*w,h/2,w/4,h/4,0,360);
  Fl_RGB_Image* img = surf->image();
  delete surf;
  current->set_current();
  return img; // return depth-3 white image on black background
}

// This callback is invoked whenever the user clicks an item in the menu bar
static void MyMenuCallback(Fl_Widget *w, void *) {
  Fl_Menu_Bar *bar = (Fl_Menu_Bar*)w;				// Get the menubar widget
  const Fl_Menu_Item *item = bar->mvalue();			// Get the menu item that was picked

  char ipath[256]; bar->item_pathname(ipath, sizeof(ipath));	// Get full pathname of picked item

  fprintf(stderr, "callback: You picked '%s'", item->label());	// Print item picked
  fprintf(stderr, ", item_pathname() is '%s'", ipath);		// ..and full pathname

  if ( item->flags & (FL_MENU_RADIO|FL_MENU_TOGGLE) ) {		// Toggle or radio item?
    fprintf(stderr, ", value is %s", item->value()?"on":"off");	// Print item's value
  }
  fprintf(stderr, "\n");
  if ( strcmp(item->label(), "Google") == 0 ) { ; }
  if ( strcmp(item->label(), "&Quit") == 0 ) { exit(0); }
}

#include "ui/imgdata.h" //main_png[]

int main(int argc, char **argv)
{
    //Fl::scheme("gtk+");
    //Fl::scheme("plastic");
    //Fl::scheme("gleam");
    const int dimW = 628+150;//751;//476;
    const int dimH = 642;//444;
    const int menubarH = MENUBARH;
    int x,y,w,h;
    Fl::screen_xywh	(x,y,w,h);
    float nScreenW = w;//Fl::w();
    float nScreenH = h;//Fl::h();
    x = (nScreenW-dimW)/2;
    y = (nScreenH-dimH)/2;
    //printf("%f,%f", nScreenW,nScreenH);

    /*初始化*/
    //glutInit(&argc,argv);
    //glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
    //glutInitDisplayMode(GLUT_DEPTH|GLUT_SINGLE|GLUT_RGB);
    //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  
    //glClearColor(255.0, 1.0 , 1.0, 255.0);//设置背景颜色为黑色  
    //glShadeModel(GL_FLAT);

    //Fl_Window window(x,y,dimW,dimH);
    CMainFrame window(x,y,dimW, dimH);//400,300
    //window.box(FL_FLAT_BOX);
    window.box(FL_NO_BOX);

    Fl_RGB_Image  icon(main_png,96,96,4);      // create the rgb image as a grayscale

//Fl_RGB_Image icon(buffer, 32, 32, 3);
//icon.color_average(c, 0.0);
    window.icon(&icon);

    window.label("他山定时 v1.0.0.7");

    //glutInitWindowPosition(cube->x()+20,cube->y()+20);
    //glutInitWindowSize(window->w()-2*cube->x(),window->h()-2*cube->y());

    //glutCreateWindow("第一个OpenGL程序");
 
    //window.resizable(g);
    //cube->hide();
    window.show(argc, argv);

    //glutMainLoop();
    Fl::run();
    return 0;
}
