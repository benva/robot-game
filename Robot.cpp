#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>

#include "VECTOR3D.h"
#include "QuadMesh.h"
#include "Room.hpp"
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
  if(left) this->angle += ROT_INC;
  else if(right) this->angle -= ROT_INC;
  
  // fetch angle in rads and calculate x and z move components
  angle = (this->angle * M_PI)/180;
  tx = MOV_INC * sin(angle);
  tz = MOV_INC * cos(angle);
  
  // Set direction vector to tx and tz components
  dir.SetX(tx);
  dir.SetZ(tz);
  
  // If displacements won't cause wall intersection, update position
  if(up) {
      if(!current_room->intersects(this, tx, tz)) {
	position.SetX(position.GetX() + tx);
	position.SetZ(position.GetZ() + tz);
      }
  }
  else if(down) {
    if(!current_room->intersects(this, -tx, -tz)) {
      position.SetX(position.GetX() - tx);
      position.SetZ(position.GetZ() - tz);
    }
  }  
}
