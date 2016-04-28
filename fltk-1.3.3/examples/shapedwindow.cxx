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

#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Tiled_Image.H>
#include <FL/Fl_Image_Surface.H>
#include "test/pixmaps/tile.xpm"
#include <FL/Fl_Menu_Bar.H>


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

const float factor = 1.3;

void shrink(Fl_Widget *wdgt, void *data)
{
  Fl_Window *win = wdgt->window();
  int old = win->w();
  win->size(old/factor, old/factor);
  if (win->w() <= *(int*)data) wdgt->deactivate();
}

void enlarge(Fl_Widget *wdgt, void *data)
{
  Fl_Window *win = wdgt->window();
  int old = win->w();
  win->size(old*factor, old*factor);
  ((Fl_Widget*)data)->activate();
}

Fl_RGB_Image* prepare_shape(int w)
{
  // draw a white circle with a hole in it on black background
  Fl_Image_Surface *surf = new Fl_Image_Surface(w, w);
  Fl_Surface_Device* current = Fl_Surface_Device::surface();
  surf->set_current();
  fl_color(FL_BLACK);
  fl_rectf(-1, -1, w+2, w+2);
  fl_color(FL_WHITE);
  fl_pie(2,2,w-4,w-4,0,360);
  fl_color(FL_BLACK);
  fl_pie(0.7*w,w/2,w/4,w/4,0,360);
  Fl_RGB_Image* img = surf->image();
  delete surf;
  current->set_current();
  return img; // return depth-3 white image on black background
}

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
int main(int argc, char **argv) {
  int dim = 200;
  Fl_Double_Window *win = new Fl_Double_Window(100, 100, dim, dim, "Testing1");
  
#if 1
    Fl_Menu_Bar *menu = new Fl_Menu_Bar(0,0,dim,25);		// Create menubar, items..
    menu->add("&File/&Open",  "^o", MyMenuCallback);
    menu->add("&File/&Save",  "^s", MyMenuCallback, 0, FL_MENU_DIVIDER);
    menu->add("&File/&Quit",  "^q", MyMenuCallback);
    menu->add("&Edit/&Copy",  "^c", MyMenuCallback);
    menu->add("&Edit/&Paste", "^v", MyMenuCallback, 0, FL_MENU_DIVIDER);
    menu->add("&Edit/Radio 1",   0, MyMenuCallback, 0, FL_MENU_RADIO);
    menu->add("&Edit/Radio 2",   0, MyMenuCallback, 0, FL_MENU_RADIO|FL_MENU_DIVIDER);
    menu->add("&Edit/Toggle 1",  0, MyMenuCallback, 0, FL_MENU_TOGGLE);			// Default: off 
    menu->add("&Edit/Toggle 2",  0, MyMenuCallback, 0, FL_MENU_TOGGLE);			// Default: off
    menu->add("&Edit/Toggle 3",  0, MyMenuCallback, 0, FL_MENU_TOGGLE|FL_MENU_VALUE);	// Default: on
    menu->add("&Help/Google",    0, MyMenuCallback);
#endif
    
  Fl_RGB_Image *img = prepare_shape(dim);
  win->shape(img);
  dragbox *box = new dragbox(0, 0, win->w(), win->h());
  box->image(new Fl_Tiled_Image(new Fl_Pixmap((const char * const *)tile_xpm)));
  Fl_Group *g = new Fl_Group(10, 20, 80, 20);
  g->box(FL_NO_BOX);
  Fl_Button *b = new Fl_Button(10, 20, 80, 20, "Close");
  b->callback(cb);
  g->end();
  g->resizable(NULL);
  g = new Fl_Group(60, 70, 80, 40, "Drag me");
  g->box(FL_NO_BOX);
  g->align(FL_ALIGN_TOP);
  Fl_Button *bs = new Fl_Button(60, 70, 80, 20, "Shrink");
  bs->callback(shrink, &dim);
  bs->deactivate();
  Fl_Button *be = new Fl_Button(60, 90, 80, 20, "Enlarge");
  be->callback(enlarge, bs);
  g->end();
  g->resizable(NULL);
  win->end();
  win->resizable(win);
  win->show(argc, argv);
  Fl::run();
  delete win;
  return 0;
}

//
// End of "$Id: shapedwindow.cxx 10325 2014-09-21 14:10:36Z manolo $".
//
