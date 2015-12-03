#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <typeinfo>

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
    hitBoxMin = VECTOR3D(-0.75, -2.0, -0.75);
    hitBoxMax = VECTOR3D(0.75, 0.0, 0.75);
    angle = 0.0;
    health = 100;
    shootTimeout = 1;
}


void Robot::draw(GLuint texid) {

    // Transform Robot before rendering its textures
    glPushMatrix();
    glTranslatef(this->getPos().GetX(), 0, this->getPos().GetZ());
    glRotatef(this->getAngle(), 0, 1, 0);

    Robot::model.draw();
    //  this->drawHitBox();
    glPopMatrix();

    //  this->drawBB();

}

void Robot::drawHitBox() {
    glPushMatrix();
    //  glColor3f(1,0,0);
    glTranslatef(0,1,0);
    glScalef(hitBoxMax.x, 1.0, hitBoxMax.z);
    glutWireCube(1.0);
    glPopMatrix();
}

void Robot::initRobot(Room * room) {
    this->current_room = room;
    position = current_room->getCenter();
    position.SetY(position.GetY() + 2.0);
}

bool Robot::move(bool up, bool down, bool left, bool right, bool stay) {
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
	if(!current_room->intersects(this, tx, tz, stay)) {
	    position.SetX(position.GetX() + tx);
	    position.SetZ(position.GetZ() + tz);
	    return true;
	}
    return false;
}

// Checks if Bullet has hit a Robot or not
bool Robot::hit(Object * b) {
    VECTOR3D bulMin,bulMax;
    VECTOR3D botMin = hitBoxMin + position;
    VECTOR3D botMax = hitBoxMax + position;
    b->getBB(&bulMin,&bulMax);
  
    if(bulMin.x < botMin.x) return false;
    if(bulMin.z < botMin.z) return false;

    if(bulMax.x > botMax.x) return false;
    if(bulMax.z > botMax.z) return false;

    return true;

}

// Reverses a Robot's directionx
void Robot::reverse() {
    setAngle(getAngle()+180);
}
