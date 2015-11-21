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
#include "Bullet.hpp"

#include "Robot.hpp"

using namespace std;

void Robot::draw(GLuint texid) {
  // Transform Robot before rendering its textures

  glPushMatrix();
  glTranslatef(this->getPos().GetX(), 0, this->getPos().GetZ());
  glRotatef(this->getAngle(), 0, 1, 0);

  // Render robot textures
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, texid);
  glBegin(GL_QUADS);

  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glTexCoord2f(0.0, 1.0);  
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(1.0f, 1.0f, -1.0f);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(1.0f, 1.0f, -1.0f);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(1.0f, -1.0f, 1.0f);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(1.0f, -1.0f, -1.0f);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glTexCoord2f( 0.0, 1.0);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(-1.0f, -1.0f, 1.0f);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-1.0f, -1.0f, 1.0f);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(1.0f, -1.0f, 1.0f);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(1.0f, -1.0f, -1.0f);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(1.0f, 1.0f, -1.0f);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(1.0f, -1.0f, -1.0f);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1.0f, -1.0f, 1.0f);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(1.0f, -1.0f, 1.0f);
  glEnd();

  glPopMatrix();

  this->drawBB();

}

void Robot::initRobot(Room * room) {
    this->current_room = room;
    position = current_room->getCenter();
    position.SetY(position.GetY() + 2.0);
}

void Robot::move(bool up, bool down, bool left, bool right) {
  float angle, tx, tz;
  if(left) this->angle += ROT_INC;
  else if(right) this->angle -= ROT_INC;
  
  //  cout << current_room << endl;
  // fetch angle in rads and calculate x and z move components
  angle = (this->angle * M_PI)/180;
  tx = MOV_INC * sin(angle);
  tz = MOV_INC * cos(angle);
  
  // Set direction vector to tx and tz components
  dir.SetX(tx);
  dir.SetZ(tz);

  if(down){
    tx *= -1;
    tz *= -1;
  }
  // If displacements won't cause wall intersection, update position
  if(up || down)
    if(!current_room->intersects(this, tx, tz)) {
      position.SetX(position.GetX() + tx);
      position.SetZ(position.GetZ() + tz);
    }

}

bool Robot::hit(Bullet * b) {
  float distance;

  distance = sqrt(pow(this->position.GetX()-b->getPos().GetX(), 2) + pow(this->position.GetY()-b->getPos().GetY(), 2) + pow(this->position.GetZ()-b->getPos().GetZ(), 2));

  // cout << "robot " << this->position.GetX() << " " << this->position.GetY() << " " << this->position.GetZ() << " " << endl;
  // cout << "bullet " << b->getPos().GetX() << " " << b->getPos().GetY() << " " << b->getPos().GetZ() << " " << endl;
  // cout << "distance " << distance << endl;
  // cout << endl;

  if(distance < 0.5)
    return true;

  return false;
}
