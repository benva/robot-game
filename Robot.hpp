// robot.hpp
// Class declaration for a Robot

#ifndef ROBOT_H
#define ROBOT_H

#include "Object.hpp"
#include "Bullet.hpp"

#define ROT_INC 1.5
#define MOV_INC 0.075

class Object;
class Room;
class Bullet;
class OBJModel;

class Robot : public Object {

protected:
  int health;
  VECTOR3D hitBoxMin, hitBoxMax;
    unsigned int shootTimeout;
public:
  static OBJModel model;
  // Constructors
  Robot(float newX=1.0, float newY=2.0, float newZ=2.0);

  ~Robot(){}  
  void initRobot(Room * room);
  
  void draw(GLuint texid);

  bool move(bool up, bool down, bool left, bool right);

  void damage(int hp) { health -= hp; }

  bool hit(Object * b);

  int getHealth(void) {return health; }

  void reverse();
  
  void drawHitBox();
};

#endif
