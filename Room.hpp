#ifndef ROOM_H
#define ROOM_H

class Room {
private:
  float height, length, width;
  VECTOR3D corner;
  VECTOR3D dir1, dir2;
  VECTOR3D ambient, diffuse, specular, shininess;
  Room* parent;
public:
  Room(Room* newParent=NULL) {parent = newParent;}
  
  bool initRoom(float height=4.0, float length=8.0, float width=6.0); 
  void draw();
  void setMaterial(VECTOR3D a, VECTOR3D d, VECTOR3D sp, VECTOR3D sh): ambient(a), diffuse(d), specular(sp), shininess(sh) {}
  
};

#endif
