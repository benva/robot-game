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
#define MESH_SIZE 16
#define DOOR_FRAME 0.5

typedef struct DoorWall {
  QuadMesh* section[3];
  QuadMesh * doorframe[4];
  float dd;
  float dw;
  float dh;
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

  // neighbor[0] is always the parent
  Room* neighbor[4];
  int parent_wall;

  DoorWall* doorwall[4];
  QuadMesh* floor;

  VECTOR3D calcNewOrigin(int wallid, VECTOR3D origin);
  VECTOR3D newDir1(int wallid, VECTOR3D up);
  void fixParentVectors(VECTOR3D * dir1v, VECTOR3D * dir2v, int pwall);
  void initDoorFrame(QuadMesh * doorframe[4], VECTOR3D dir1v, VECTOR3D dir2v, VECTOR3D origin, float dh, float dw);
public:
  Room(Room* newParent=NULL, int pwall=2) {neighbor[0] = newParent; parent_wall = pwall;}
  ~Room() {}
  bool initRoom(float newLength=3.0, float newWidth=4.0, float newHeight=2.0); 
  void draw();
  bool addDoor(int wallid, float dd=1.0, float dh=1.5, float dwidth=1.0);
  bool addNeighbor(Room* newNeighbor, int wallid);
};

#endif
