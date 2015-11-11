// robot.h
// Class declaration for a Robot

#ifndef ROBOT_H
#define ROBOT_H

class Robot {
private:
  float x, y, z;
  float angle;
  int health;
public:
  // Constructors
  Robot(float newX=0.0, float newY=0.0, float newZ=0.0) : x(newX), y(newY), z(newZ), angle(0.0), health(100) {}

  ~Robot(){}  

  void draw();
  void set(float newX, float newY, float newZ) {x = newX; y = newY; z = newZ; }
  void setAngle(float newAngle) {angle = newAngle; }
  void damage(int hp) { health -= hp; }

  float getX(void) {return x;}
  float getY(void) {return y;}
  float getZ(void) {return z;}
  float getAngle(void) {return angle;}
  int getHealth(void) {return health; }
};

#endif
