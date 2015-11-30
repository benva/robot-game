#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <vector>

#include "VECTOR3D.h"
#include "QuadMesh.h"
#include "Room.hpp"
#include "Bullet.hpp"

#include "OBJModel.hpp"

#include "Robot.hpp"

using namespace std;

OBJModel Robot::model;

Robot::Robot(float newX, float newY, float newZ) { 
   set(newX,newY,newZ);
    dir.LoadZero();
    minBB = VECTOR3D(-0.25, -2.0, -0.25);
    maxBB = VECTOR3D(0.25, 0.0, 0.25);
    angle = 0.0;
    health = 100;
}


void Robot::draw(GLuint texid) {
  // Transform Robot before rendering its textures

  glPushMatrix();
  glTranslatef(this->getPos().GetX(), 0, this->getPos().GetZ());
  glRotatef(this->getAngle(), 0, 1, 0);

  Robot::model.draw();

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
  float dist1, dist2, distance;

  dist1 = sqrt(pow(this->position.GetX(), 2) + pow(this->position.GetY(), 2) + pow(this->position.GetZ(), 2));
  dist2 = sqrt(pow(b->getPos().GetX(), 2) + pow(b->getPos().GetY(), 2) + pow(b->getPos().GetZ(), 2));
  distance = fabs(dist1 - dist2);

  if(distance <= 0.1)
    return true;

  return false;
}
