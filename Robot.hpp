// robot.hpp
// Class declaration for a Robot

#ifndef ROBOT_H
#define ROBOT_H

#define ROT_INC 2.0
#define MOV_INC 0.1

class Room;
class Robot {
private:
  VECTOR3D position;
  float angle;
  int health;
  VECTOR3D dir;
  Room * current_room;
  VECTOR3D minBB, maxBB;

  void drawBB(void);
public:
  // Constructors
  Robot(float newX=1.0, float newY=2.0, float newZ=2.0) : angle(0.0), health(100) {
    set(newX,newY,newZ);
    dir.LoadZero();
    minBB = VECTOR3D(-0.25, -2.0, -0.25);
    maxBB = VECTOR3D(0.25, 0.0, 0.25);
  }

  ~Robot(){}  
  void initRobot(Room * room);

  void draw(GLuint texid);

  void move(bool up, bool down, bool left, bool right);


  void set(float newX, float newY, float newZ) {
    position.SetX(newX);
    position.SetY(newY); 
    position.SetZ(newZ);
  }
  void setAngle(float newAngle) {angle = newAngle; }
  void damage(int hp) { health -= hp; }

  float getX(void) {return position.GetX();}
  float getY(void) {return position.GetY();}
  float getZ(void) {return position.GetZ();}
  
  VECTOR3D getPos(void) {return position;}
  VECTOR3D getDir(void) {return dir;}

  void getBB(VECTOR3D * minBB, VECTOR3D * maxBB);

  float getAngle(void) {return angle;}
  int getHealth(void) {return health; }

  
};

#endif
