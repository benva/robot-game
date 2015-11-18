#include <iostream>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#include "VECTOR3D.h"
#include "QuadMesh.h"
#include "Room.hpp"
#include "Bullet.hpp"

void Bullet::draw(GLuint texid) {
	glPushMatrix();

	glTranslatef(this->getPos().GetX(), 1.5, this->getPos().GetZ());

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  gluQuadricTexture(q, 1);
  glBindTexture(GL_TEXTURE_2D, texid);
  gluSphere(q, 0.2, 20, 30);

  glPopMatrix();
}

void Bullet::move() {
  if(current_room->intersects(this, position.GetX(), position.GetZ()))
    delete this;

  position += dir * BUL_INC;
}
