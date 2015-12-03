#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <list>

#include "VECTOR3D.h"
#include "QuadMesh.h"

#include "Object.hpp"
#include "Bullet.hpp"
#include "Robot.hpp"
#include "Room.hpp"
#include "Avatar.hpp"
#include "EvilRobot.hpp"

#define ROT_INC_E 1
// Makes the bot move around its current room randomly
void EvilRobot::move() {
  int dir;
  bool left, right;

  left = right = false;
  srand(time(NULL));
    
  dir = rand() % 2;
  dir == 1 ? left = true : right = true;

  collision();  

  // While bot can't move forward, turn it left or right randomly
  if(!((Robot*)this)->move(true, false, false, false))
      ((Robot*)this)->move(false, false, left, right);
}

// Checks if any bots are colliding and if so changes their trajectories
void EvilRobot::collision() {
  // Obtains a list of the other bots in the room
  list<EvilRobot*> bots = this->current_room->getBots();

  for(list<EvilRobot*>:: iterator it=bots.begin(); it!=bots.end(); ++it) {
    if((*it) != this && (*it)->hit(this)) {
      (*it)->reverse();
      this->reverse();
    }
  }
}

// Makes the bot face the avatar and fire
void EvilRobot::attack(Avatar * avatar) {
  Bullet * bullet;
  VECTOR3D dir;
  float targAngle,currAngle,angle;
  float rot;
  float thresh = 1.5;
  dir = avatar->getPos() - this->getPos();

  targAngle = atan2f(dir.x, dir.z)*180/M_PI;
  currAngle = atan2f(this->getDir().x, this->getDir().z)*180/M_PI;
  
  cout << currAngle << " " << targAngle << endl;
  
  if(abs(targAngle-currAngle) < 180) {
      if(targAngle+thresh < currAngle) {
	  this->angle -= ROT_INC_E;
      } else if (targAngle-thresh > currAngle) {
	  this->angle += ROT_INC_E;
      }
  }else {
      if(targAngle+thresh < currAngle) {
	  this->angle += ROT_INC_E;
      } else if (targAngle-thresh > currAngle) {
	  this->angle -= ROT_INC_E;
      }
  }
  angle = (this->angle*M_PI)/180;
  this->dir.SetX(MOV_INC*sin(angle));
  this->dir.SetZ(MOV_INC*cos(angle));      

  bullet = new Bullet(this);
  // bullets.push_back(bullet);
}
