#ifndef OBJECT_H
#define OBJECT_H

class Room;
class Object {
protected:
  VECTOR3D position;
  float angle;
  VECTOR3D dir;
  Room * current_room;
public:
  float getX(void) {return position.GetX();}
  float getY(void) {return position.GetY();}
  float getZ(void) {return position.GetZ();}

  void set(float newX, float newY, float newZ) {
    position.SetX(newX);
    position.SetY(newY); 
    position.SetZ(newZ);
  }
  
  VECTOR3D getPos(void) {return position;}
  VECTOR3D getDir(void) {return dir;}

  void setAngle(float newAngle) {angle = newAngle; }
  float getAngle(void) {return angle;}

  Room * getCurrentRoom() { return current_room; }
  void setCurrentRoom(Room * room) { current_room = room; }

};

#endif
