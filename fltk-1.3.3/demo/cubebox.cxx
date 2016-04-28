
#include"cubebox.h"

#if HAVE_GL
/* The cube definition */
float v0[3] = {0.0, 0.0, 0.0};
float v1[3] = {1.0, 0.0, 0.0};
float v2[3] = {1.0, 1.0, 0.0};
float v3[3] = {0.0, 1.0, 0.0};
float v4[3] = {0.0, 0.0, 1.0};
float v5[3] = {1.0, 0.0, 1.0};
float v6[3] = {1.0, 1.0, 1.0};
float v7[3] = {0.0, 1.0, 1.0};
void drawcube(int wire) {
/* Draw a colored cube */
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(0,0,255);
  v3f(v0); v3f(v1); v3f(v2); v3f(v3);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(0,255,255); v3f(v4); v3f(v5); v3f(v6); v3f(v7);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(255,0,255); v3f(v0); v3f(v1); v3f(v5); v3f(v4);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(255,255,0); v3f(v2); v3f(v3); v3f(v7); v3f(v6);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(0,255,0); v3f(v0); v3f(v4); v3f(v7); v3f(v3);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(255,0,0); v3f(v1); v3f(v2); v3f(v6); v3f(v5);
  glEnd();
}

void cube_box::draw() {
  lasttime = lasttime+speed;
  if (!valid()) {
    glLoadIdentity();
    glViewport(0,0,w(),h());
    glEnable(GL_DEPTH_TEST);
    glFrustum(-1,1,-1,1,2,10000);
    glTranslatef(0,0,-10);
    gl_font(FL_HELVETICA_BOLD, 16 );
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  glRotatef(float(lasttime*1.6),0,0,1);
  glRotatef(float(lasttime*4.2),1,0,0);
  glRotatef(float(lasttime*2.3),0,1,0);
  glTranslatef(-1.0, 1.2f, -1.5);
  glScalef(float(size),float(size),float(size));
  drawcube(wire);
  glPopMatrix();
  gl_color(FL_GRAY);
  glDisable(GL_DEPTH_TEST);
  gl_draw(wire ? "Cube: wire" : "Cube: flat", -4.5f, -4.5f );
  glEnable(GL_DEPTH_TEST);
}

int cube_box::handle(int e) {
  switch (e) {
  case FL_ENTER: cursor(FL_CURSOR_CROSS); break;
  case FL_LEAVE: cursor(FL_CURSOR_DEFAULT); break;
  }
  return Fl_Gl_Window::handle(e);
}


#endif

