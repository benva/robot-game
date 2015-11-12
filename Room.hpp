#ifndef ROOM_H
#define ROOM_H

/*
initRoom(h,l,w) 
addDoor(wallid, dw, dd, dh)

top view:
   ________
  |   2    |
w |3      1|
  |->_0____|
o     l

wall view: 
     ___________
    |   __      |
hgt |  |  |dh   |
    |->|  |_____|
  o  dd dw

*/
typedef struct DoorWall {
  QuadMesh* section[3];
  float dd;
  float dwidth;
  float dh;
  float size[3];
  float len[3];
  float hgt[3];
  VECTOR3D origin[3];
  float length;
  VECTOR3D dir1v, dir2v;
}DoorWall;

class Room {
private:
  float height, length, width;
  float dd;
  float dw;
  VECTOR3D origin;
  VECTOR3D dir1v, dir2v;
  Room* parent;
  DoorWall* doorwall[4]={NULL,NULL,NULL,NULL};
  QuadMesh* floor;

  VECTOR3D calcNewOrigin(int wallid);
  VECTOR3D newDir1(int wallid, VECTOR3D up);
public:
  Room(Room* newParent=NULL) {parent = newParent;}
  ~Room() {}

  bool initRoom(float newHeight=1.0, float newLength=8.0, float newWidth=6.0); 
  void draw();
  bool addDoor(int wallid, float dw, float dd, float dh);

};

#endif
