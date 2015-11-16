#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#include "VECTOR3D.h"
#include "Robot.hpp"


void Robot::draw(GLuint texid) {
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glPushMatrix();

  glBindTexture(GL_TEXTURE_2D, texid);
  glBegin(GL_QUADS);
  glTexCoord2f( 0.0, 0.0);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glTexCoord2f( 0.0, 1.0);  
  glVertex3f(-1.0f, 1.0f,  1.0f);
  glTexCoord2f( 1.0, 1.0);
  glVertex3f( 1.0f, 1.0f,  1.0f);
  glTexCoord2f( 1.0, 0.0);
  glVertex3f( 1.0f, 1.0f, -1.0f);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f( 0.0, 0.0);
  glVertex3f( 1.0f, 1.0f, -1.0f);
  glTexCoord2f( 0.0, 1.0);
  glVertex3f( 1.0f, 1.0f,  1.0f);
  glTexCoord2f( 1.0, 1.0);
  glVertex3f( 1.0f, -1.0f,  1.0f);
  glTexCoord2f( 1.0, 0.0);
  glVertex3f( 1.0f, -1.0f, -1.0f);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f( 0.0, 0.0);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glTexCoord2f( 0.0, 1.0);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glTexCoord2f( 1.0, 1.0);
  glVertex3f(-1.0f, -1.0f,  1.0f);
  glTexCoord2f( 1.0, 0.0);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f( 0.0, 0.0);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glTexCoord2f( 0.0, 1.0);
  glVertex3f(-1.0f, -1.0f,  1.0f);
  glTexCoord2f( 1.0, 1.0);
  glVertex3f( 1.0f, -1.0f,  1.0f);
  glTexCoord2f( 1.0, 0.0);
  glVertex3f( 1.0f, -1.0f, -1.0f);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f( 0.0, 0.0);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glTexCoord2f( 0.0, 1.0);
  glVertex3f( -1.0f, 1.0f,  -1.0f);
  glTexCoord2f( 1.0, 1.0);
  glVertex3f( 1.0f, 1.0f,  -1.0f);
  glTexCoord2f( 1.0, 0.0);
  glVertex3f( 1.0f, -1.0f, -1.0f);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f( 0.0, 0.0);
  glVertex3f(-1.0f, -1.0f, 1.0f);
  glTexCoord2f( 0.0, 1.0);
  glVertex3f(-1.0f, 1.0f,  1.0f);
  glTexCoord2f( 1.0, 1.0);
  glVertex3f( 1.0f, 1.0f,  1.0f);
  glTexCoord2f( 1.0, 0.0);
  glVertex3f( 1.0f, -1.0f, 1.0f);
  glEnd();

  glPopMatrix();
}

void Robot::move(bool up, bool down, bool left, bool right) {
  float angle, tx, tz;
  if(left) this->angle -= ROT_INC;
  if(right) this->angle += ROT_INC;
  
  angle = (this->angle * M_PI)/180;
  tx = MOV_INC * sin(angle);
  tz = MOV_INC * cos(angle);
  
  if(up) {
    position.SetX(position.GetX() + tx);
    position.SetX(position.GetZ() + tz);
  }
  else if(down) {
    position.SetX(position.GetX() - tx);
    position.SetX(position.GetZ() - tz);
  }
  
}
