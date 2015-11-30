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

bool Robot::move(bool up, bool down, bool left, bool right) {
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
      return true;
    }
  return false;
}

// Checks if Bullet has hit a Robot or not
bool Robot::hit(Object * b) {
  float distance, dx, dy, dz;

  dx = this->getPos().GetX() - b->getPos().GetX();
  dy = this->getPos().GetY() - b->getPos().GetY();
  dz = this->getPos().GetZ() - b->getPos().GetZ();
  distance = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));

  // cout << "robot " << this->position.GetX() << " " << this->position.GetY() << " " << this->position.GetZ() << " " << endl;
  // cout << "bullet " << b->getPos().GetX() << " " << b->getPos().GetY() << " " << b->getPos().GetZ() << " " << endl;
  // cout << "distance " << distance << endl;
  // cout << endl;

  if(distance < 0.5)
    return true;

  return false;
}

// Reverses a Robot's direction
void Robot::reverse() {
  setAngle(getAngle()+180);
}
