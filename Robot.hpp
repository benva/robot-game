// robot.hpp
// Class declaration for a Robot

#ifndef ROBOT_H
#define ROBOT_H

#include "Object.hpp"

#define ROT_INC 2.0
#define MOV_INC 0.1

class Room;
class Robot : public Object{
private:
  int health;
  VECTOR3D minBB, maxBB;

  void drawBB(void);
public:
  // Constructors
  Robot(float newX=1.0, float newY=2.0, float newZ=2.0) : health(100) {
    set(newX,newY,newZ);
    dir.LoadZero();
    minBB = VECTOR3D(-0.25, -2.0, -0.25);
    maxBB = VECTOR3D(0.25, 0.0, 0.25);
    angle = 0.0;
  }

  ~Robot(){}  
  void initRobot(Room * room);
  
  void draw(GLuint texid);

  void move(bool up, bool down, bool left, bool right);

  void damage(int hp) { health -= hp; }

  void getBB(VECTOR3D * minBB, VECTOR3D * maxBB);

  int getHealth(void) {return health; }
};

#endif
