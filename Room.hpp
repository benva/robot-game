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
#define MAX_MESH_SIZE 16
#define DOOR_FRAME 0.5

#include <list>

class Object;
class Robot;
class Avatar;
class EvilRobot;
class Bullet;

typedef struct TextureQuad {
    VECTOR3D origin;
    VECTOR3D dir1v;
    VECTOR3D dir2v;
    float width;
    float length;
}TextureQuad;

typedef struct DoorWall {
    QuadMesh* section[3];
    QuadMesh* doorframe[4];
  
    TextureQuad * tsection[3];
    TextureQuad * tdoorframe[4];

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
    TextureQuad* tfloor;

    GLuint floor_texture;
    GLuint wall_texture;
    GLuint door_texture;

    list<EvilRobot*> bots;
    list<Bullet*> bullets;

    unsigned int maxBots;
    int timetospawn;

    VECTOR3D calcNewOrigin(int wallid, VECTOR3D origin);
    VECTOR3D calcNewOrigin(int wallid, VECTOR3D origin, Room * parent);
    VECTOR3D newDir1(int wallid, VECTOR3D up);
    void fixParentVectors(VECTOR3D * dir1v, VECTOR3D * dir2v, int pwall);
    void initDoorFrame(DoorWall * dw, VECTOR3D dir1v, VECTOR3D dir2v, VECTOR3D origin, float dh, float dwidth);
    void drawTexture(TextureQuad * tq, GLuint texid, float mult1, float mult2);
    TextureQuad * makeTQ(VECTOR3D origin, float length, float width, VECTOR3D dir1v, VECTOR3D dir2v);
    int getRoomBB(VECTOR3D * minRoom, VECTOR3D * maxRoom);
    bool within_doorway(int wall_dir, int wall_id, VECTOR3D * minBB, VECTOR3D * maxBB);
    void collision(EvilRobot * bot);

public:
    Room(Room* newParent=NULL, int pwall=2) {neighbor[0] = newParent; parent_wall = pwall; neighbor[1] = NULL; neighbor[2] = NULL; neighbor[3] = NULL;}
    ~Room();
    bool initRoom(float newLength=7.0, float newWidth=7.0, float newHeight=4.0); 
    void draw();
    bool addDoor(int wallid, float dd=1.0, float dh=2.5, float dwidth=1.5);
    bool addNeighbor(Room* newNeighbor, int wallid);
    void setTextures(GLuint wall, GLuint floor, GLint door=-1) { wall_texture = wall; floor_texture = floor; door==-1? door_texture = wall : door_texture = door; }
    bool intersects(Object * o, float tx, float tz, bool stay);
    VECTOR3D getCenter(void);
    Room * getNeighbor(int i) { return neighbor[i]; }
    void setNeighbor(int i, Room * room) { neighbor[i] = room; }
    void goodbye(Room * room);
    bool hitbot(Bullet * bul);
    void newBot();
    list<EvilRobot*> getBots() { return bots; }
    void move(Avatar * avatar);
    void attack(Avatar * avatar, list<Bullet*> * bullets);
    void setTimeToSpawn(int t2s) { this->timetospawn = t2s; }
    bool empty();
};

#endif
