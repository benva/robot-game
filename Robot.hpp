// robot.h
// Class declaration for a Robot

#ifndef ROBOT_H
#define ROBOT_H



class Robot {
private:
  VECTOR3D position;
  float angle;
  int health;
public:
  // Constructors
  Robot(float newX=0.0, float newY=0.0, float newZ=0.0) : angle(0.0), health(100) {
    set(newX,newY,newZ);
  }

  ~Robot(){}  

  void draw(GLuint texid);
  void moveForward(float d);
  void moveBackward(float d);
  void rotate(float angle);
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
  float getAngle(void) {return angle;}
  int getHealth(void) {return health; }
};

#endif
