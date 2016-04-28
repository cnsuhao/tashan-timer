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
#include <FL/Fl_Gl_Window.H>

#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Tiled_Image.H>
#include <FL/Fl_Image_Surface.H>
#include "test/pixmaps/tile.xpm"
#include "cubebox.h"

class MYGLWindow : public Fl_Gl_Window
{
    void draw(); //作图操作
    virtual int handle( int ); //消息事件处理
public :
    MYGLWindow(int x,int y,int w,int h,const char *L) : Fl_Gl_Window(x,y,w,h,L){};
};

int MYGLWindow::handle( int event) //事件处理
{  
    switch (event)
    {  
        case FL_PUSH :  //操作等
             return 1;
    }
    return 1;
}

void MYGLWindow::draw() //作图
{
    if ( !valid() )
    {
        //设置viewport窗口大小等等 例如
        /**********************************************
        valid(1);
        glLoadIdentity();
        glViewport(0,0,w(),h());
        ***********************************************/
    }
    //添加使用OPENGL作图操作
    /** ……..*/   
};

void cb(Fl_Widget *w, void *) {
  w->window()->hide();
}

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
};



Fl_RGB_Image* prepare_shape(int w, int h)
{
  // draw a white circle with a hole in it on black background
  Fl_Image_Surface *surf = new Fl_Image_Surface(w, h);
  Fl_Surface_Device* current = Fl_Surface_Device::surface();
  surf->set_current();
  fl_color(FL_BLACK);
  fl_rectf(-1, -1, w+2, h+2);
  fl_color(FL_WHITE);
  fl_pie(2,2,w-4,h-4,0,360);
  fl_color(FL_BLACK);
  fl_pie(0.7*w,w/2,w/4,h/4,0,360);
  Fl_RGB_Image* img = surf->image();
  delete surf;
  current->set_current();
  return img; // return depth-3 white image on black background
}

int main(int argc, char **argv) {
    int x,y,w,h;
    Fl::screen_xywh	(x,y,w,h);
    float nScreenW = w;//Fl::w();
    float nScreenH = h;//Fl::h();
    //Fl::screen_dpi(nScreenW, nScreenH);
    printf("%f,%f", nScreenW,nScreenH);

    Fl_Window *window = new Fl_Window(476,444);//400,300

    cube_box *cube;
    //cube = new cube_box(23,3,354,280, 0);
    cube = new cube_box(23,3,430, 420, 0);
    cube->speed = 1.0;
    cube->size = 1.0;
    cube->wire = 0; 

    window->label("Demo for Fedora");
    window->end();
    window->resizable(window);
    window->show(argc, argv);

    cube->show();

    for (;;) 
    {
        if (window->visible())
        {if (!Fl::check()) break;}	// returns immediately
        else
        {if (!Fl::wait()) break;}	// waits until something happens

        cube->redraw();
        //if (Fl::readqueue() == button) break;
    }//for

    Fl::run();
    delete window;
    return 0;
}

int main3(int argc, char **argv) 
{
    int dimW = 400, dimH=300;
    int x,y;

    int nScreenW = 0;//Fl::w();
    int nScreenH = 0;//Fl::h();
    Fl::screen_xywh	(x,y,nScreenW,nScreenH);

    float nOffsetX = ((float)nScreenW-dimW)/2.0;
    float nOffsetY = ((float)nScreenH-dimH)/2.0;
    printf("%f,%f", nOffsetX, nOffsetY );
    
    cube_box *cube;
    cube = new cube_box(23,23,344,344, 0);
    cube->speed = 1.0;
    cube->size = 1.0;
    cube->wire = 0; 


    Fl_Double_Window *win = new Fl_Double_Window(nOffsetX, nOffsetY, dimW, dimH, "Testing1");
    //Fl_RGB_Image *img = prepare_shape(dimW, dimH);
    //win->shape(img);
    dragbox *box = new dragbox(0, 0, win->w(), win->h());
    box->image(new Fl_Tiled_Image(new Fl_Pixmap((const char * const *)tile_xpm)));


  Fl_Box *b = new Fl_Box(FL_NO_BOX,cube->x(),100,
			 cube->w(),100,0);
  win->resizable(b);
  b->hide();

    win->label("Demo");
    win->end();
    win->resizable(win);
    win->show(argc, argv);
    cube->show();

    for (;;) 
    {
        if (win->visible())
        {if (!Fl::check()) break;}	// returns immediately
        else
        {if (!Fl::wait()) break;}	// waits until something happens

        cube->redraw();
        //if (Fl::readqueue() == button) break;
    }//for

    Fl::run();
    delete win;
    return 0;
}

//
// End of "$Id: shapedwindow.cxx 10325 2014-09-21 14:10:36Z manolo $".
//
