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
#include "Robot.hpp"

#include "Bullet.hpp"

Bullet::Bullet(Robot * r) {
    dir = r->getDir();
    position = r->getPos();
    q = gluNewQuadric();
    minBB = VECTOR3D(-0.1,0,-0.1);
    maxBB = VECTOR3D(0.1,0,0.1);
    current_room = r->getCurrentRoom();
}

void Bullet::draw(GLuint texid) {
  glPushMatrix();

  glTranslatef(this->getPos().GetX(), 1.5, this->getPos().GetZ());

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  gluQuadricTexture(q, 1);
  glBindTexture(GL_TEXTURE_2D, texid);
  gluSphere(q, 0.2, 20, 30);

  glPopMatrix();

  this->drawBB();
}

bool Bullet::move() {
  if(current_room->intersects(this, (dir*BUL_INC).GetX(), (dir*BUL_INC).GetZ()))
    return false;
  
  position += dir * BUL_INC;
  return true;
}
