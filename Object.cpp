#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#include "VECTOR3D.h"
#include "Object.hpp"

// verrrry simple BB implementation
void Object::getBB(VECTOR3D * minBB, VECTOR3D * maxBB) {
  *minBB = this->minBB + position;
  *maxBB = this->maxBB + position;
}

void Object::drawBB(void) {
  VECTOR3D min,max;
  getBB(&min,&max);
  //  cout << "Min: " << min.GetX() << " " << min.GetY() << " " << min.GetZ() << endl;
  //  cout << "Max: " << max.GetX() << " " << max.GetY() << " " << max.GetZ() << endl<<endl;
  
  glPushMatrix();
  glTranslatef(position.GetX(),1.0,position.GetZ());
  glScalef(1.0,4.0,1.0);
  glutWireCube(0.5);
  glPopMatrix();
}


void Object::setDir(VECTOR3D newDir) {
  float newAngle;

  this->dir = newDir;
  newAngle = fabs(atan2f(dir.GetX(), dir.GetZ()))*180/M_PI;

  cout << "newAngle: " << newAngle;
  cout << " currentAngle: " << this->getAngle() << endl;

  this->setAngle(newAngle);
}