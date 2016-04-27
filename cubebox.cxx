
#include"cubebox.h"
#include <GL/gl.h>
#include <GL/glu.h>  
#include <GL/glut.h> 
#include <FL/fl_draw.H>
#include <FL/Fl_Text_Display.H>


#if HAVE_GL
cube_box::cube_box(int l, int t, int w, int h, const char* name)
    :Fl_Gl_Window(l,t,w,h,name)
{
    init();
}

cube_box::~cube_box()
{
}

void cb00(Fl_Widget *w, void *) {
  w->window()->hide();
}

static char s[255];
void myDisplay2(void)
{
glClear(GL_COLOR_BUFFER_BIT);
glColor3f( 0.0, 1.0, 0.0 );
glRectf(-0.5f,-0.5f,0.5f,0.5f);
glFlush();
}
void cube_box::init()
{
  when(FL_WHEN_RELEASE);
    lasttime = 0.0;   
  Fl_Group *g = new Fl_Group(10, 8, 80, 20);
  g->box(FL_NO_BOX);
  Fl_Button *b = new Fl_Button(10, 8, g->w(), g->h(), "Close");

  g->end();
  g->resizable(NULL);
  mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);
}

//http://research.cs.wisc.edu/graphics/Courses/559-f2004/tutorials/tutorial3.html
void cube_box::InitializeGL()
{
   glClearColor(.1f, .1f, .1f, 1);
   glEnable(GL_DEPTH_TEST);

   glClearColor(0.5f,0.5f,1.0f,0.0f);

}

void cube_box::DrawCube()
{
   glBegin(GL_QUADS);
      // front
      glColor3f(1, 0, 0);
      glVertex3f(-1, 1, 1);
      glVertex3f(-1, -1, 1);
      glVertex3f(1, -1, 1);
      glVertex3f(1, 1, 1);

      // back
      glColor3f(0, 1, 0);
      glVertex3f(-1, 1, -1);
      glVertex3f(1, 1, -1);
      glVertex3f(1, -1, -1);
      glVertex3f(-1, -1, -1);

      // top
      glColor3f(0, 0, 1);
      glVertex3f(-1, 1, -1);
      glVertex3f(-1, 1, 1);
      glVertex3f(1, 1, 1);
      glVertex3f(1, 1, -1);

      // bottom
      glColor3f(1, 1, 0);
      glVertex3f(-1, -1, -1);
      glVertex3f(1, -1, -1);
      glVertex3f(1, -1, 1);
      glVertex3f(-1, -1, 1);

      // left
      glColor3f(0, 1, 1);
      glVertex3f(-1, 1, -1);
      glVertex3f(-1, -1, -1);
      glVertex3f(-1, -1, 1);
      glVertex3f(-1, 1, 1);

      // right
      glColor3f(1, 0, 1);
      glVertex3f(1, 1, 1);
      glVertex3f(1, -1, 1);
      glVertex3f(1, -1, -1);
      glVertex3f(1, 1, -1);
   glEnd();
}

void cube_box::draw_overlay() 
{
snprintf(s, 255,"世界你好！");
fl_color(FL_BLUE);
fl_font(FL_HELVETICA, 14);
fl_draw(s, 0, 20, w() - 40, 20, FL_ALIGN_LEFT);
}

void cube_box::draw() 
{
   static bool firstTime = true;
   if (firstTime)
   {
       //InitializeGL();
      firstTime = false;
   }// if
    glClear(GL_COLOR_BUFFER_BIT);
   glBegin(GL_TRIANGLES);


glEnd();
    glFlush();
}

int cube_box::handle(int e) {
  switch (e) {
  case FL_ENTER: cursor(FL_CURSOR_CROSS); break;
  case FL_LEAVE: cursor(FL_CURSOR_DEFAULT); break;
  }
  return Fl_Gl_Window::handle(e);
}

#endif

