#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <time.h>

#include "RGBpixmap.h"
#include "VECTOR3D.h"
#include "QuadMesh.h"
#include "EvilRobot.hpp"
//#include "Robot.hpp"


#include "Room.hpp"

using namespace std;

Room::~Room() {
  int i;
  for(i=0;i<4;i++)
    if(neighbor[i] != NULL)
      neighbor[i]->goodbye(this);
  for(i=0;i<4;i++)
    free(doorwall[i]);
  free(floor);
  free(tfloor);
}

void Room::goodbye(Room * room) {
  int i;
  for(i=0;i<4;i++)
    if(neighbor[i] == room) {
      addDoor(i,-1,-1,-1);
      neighbor[i] = NULL;
      return;
    }
}

void Room::draw() {
  int i,j;

  drawTexture(tfloor, floor_texture,0.5,0.5);

  //floor->DrawMesh(MAX_MESH_SIZE);

  for(i=0;i<4;i++) {
    if(doorwall[i]->dd == -1){
      drawTexture(doorwall[i]->tsection[0], wall_texture,1,1);
      doorwall[i]->section[0]->DrawMesh(MAX_MESH_SIZE);
    } else {
      for(j=0; j<3; j++) {
	drawTexture(doorwall[i]->tsection[j], wall_texture,1,1);
	doorwall[i]->section[j]->DrawMesh(MAX_MESH_SIZE);
      }
      for(j=0; j<4; j++) {
	drawTexture(doorwall[i]->tdoorframe[j], door_texture,1,1);
	doorwall[i]->doorframe[j]->DrawMesh(MAX_MESH_SIZE);
      }
    }
  }
  for(list<EvilRobot*>::iterator it=bots.begin(); it!=bots.end(); ++it)
    (*it)->draw(2000);

}

bool Room::initRoom(float newLength, float newWidth, float newHeight) {
  int i;
  height = newHeight;
  length = newLength;
  width = newWidth;

  floor_texture = 2000;
  wall_texture = 2001;
  door_texture = 2001;

  // If this room is the head of the roomtree, set values accordingly
  if(neighbor[0] == NULL) {
    dir1v = VECTOR3D(1.0f, 0.0f, 0.0f);
    dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
    //    origin = VECTOR3D(0.0f, 0.0f, 0.0f);
    origin = VECTOR3D(0.0f, 0.0f, 0.0f);
  }
  else {
    // set room vectors to parent
    dir1v = neighbor[0]->dir1v;
    dir2v = neighbor[0]->dir2v;

    // fetch origin from parent and calc adjusted one based on wall of parent
    origin = calcNewOrigin((parent_wall+1)%4, neighbor[0]->origin, neighbor[0]);
    // fix parent vectors AFTER origin has been calculated
    fixParentVectors(&dir1v,&dir2v,parent_wall);
    origin += dir2v*DOOR_FRAME;
    // let parent know where you are
    neighbor[0]->addNeighbor(this, parent_wall);

  }
  // Create doorless walls for room
  for(i=0;i<4;i++)
    addDoor(i,-1,-1,-1);
 
  // If there is a parent, make a door with it
  if(neighbor[0] != NULL) {
    // Case where parent doesn't have a door yet on connecting wall
    if(neighbor[0]->doorwall[parent_wall]->dd == -1) {
      // dd will need to be different for each, dh and dwidth stay same
      float len_child = doorwall[0]->length;
      float len_parent = neighbor[0]->doorwall[parent_wall]->length;
      // if parent is wider = width_child/2

      if(len_child <= len_parent) {
	addDoor(0,len_child/2);
	neighbor[0]->addDoor(parent_wall, len_parent-len_child/2);
	// if child is wider= width_parent/2
      } else {
	neighbor[0]->addDoor(parent_wall, len_parent/2);
	addDoor(0, len_parent/2);
      }
    }
  }

  floor = new QuadMesh(MAX_MESH_SIZE);
  floor->InitMesh(MESH_SIZE, origin, length, width, dir1v, dir2v);
  tfloor = makeTQ(origin,width,length,dir2v,dir1v);

  return true;
}

// adds a door dd from origin, rotated to wallid, of height dh, and width dwidth
// dd is displacement from corner to centre of door
bool Room::addDoor(int wallid, float dd, float dh, float dwidth) {
  int i;
  doorwall[wallid] = (DoorWall*) calloc (1,sizeof(DoorWall));
  DoorWall * dw = doorwall[wallid];

  // make length of wall appropriate dimension
  dw->length = wallid%2 == 0? length : width;
  
  // set origin based on wall number and room origin
  dw->origin[0] = calcNewOrigin(wallid, origin);

  // calculate new vectors for the wall
  dw->dir2v = dir1v.CrossProduct(dir2v);
  dw->dir1v = newDir1(wallid, dw->dir2v);
  
  // Set door displacement, door height, door width
  dw->dd = dd;
  dw->dh = dh;
  dw->dw = dwidth;

  // If this wall is supposed to have a door, set the 3 panel dimensions and origins
  if(dd != -1) {
    // Calculate origin of other two panels
    dw->len[0] = dd-dwidth/2;
    dw->len[1] = dwidth;
    dw->len[2] = dw->length-dd-dwidth/2;
    dw->hgt[0] = height;
    dw->hgt[1] = height-dh;
    dw->hgt[2] = height;

    dw->origin[1] = dw->origin[0] + dw->dir1v*dw->len[0] + dw->dir2v*dh;
    dw->origin[2] = dw->origin[0] + dw->dir1v*(dw->len[0]+dwidth);
  }


  // If this wall is supposed to have a door, create 3 new quadmeshes based on dimensions
  if(dd != -1) {
    for(i=0;i<3;i++) {
      dw->section[i] = new QuadMesh(MAX_MESH_SIZE);
      dw->section[i]->InitMesh(MESH_SIZE, dw->origin[i], dw->len[i], dw->hgt[i], dw->dir1v,dw->dir2v);
      dw->tsection[i] = makeTQ(dw->origin[i], dw->len[i], dw->hgt[i], dw->dir1v, dw->dir2v);
    }
    // Create doorframe meshes
    initDoorFrame(dw, dw->dir1v, dw->dir2v, dw->origin[1], dw->dh, dw->dw);
    // If this is a doorless wall, create just the first quadmesh
  } else {
    dw->section[0] = new QuadMesh(MAX_MESH_SIZE);
    dw->section[0]->InitMesh(MESH_SIZE, dw->origin[0], dw->length, height, dw->dir1v,dw->dir2v);
    dw->tsection[0] = makeTQ(dw->origin[0], dw->length, height, dw->dir1v,dw->dir2v);
  }    

  return true;
}

// returns origin of each corner based on the wall id
VECTOR3D Room::calcNewOrigin(int wallid, VECTOR3D origin) {
  if(wallid == 1) return origin + (dir1v*length);
  if(wallid == 2) return origin + (dir1v*length) + (dir2v*width);
  if(wallid == 3) return origin + (dir2v*width);
  return origin;
}

// returns origin of each corner based on the wall id
VECTOR3D Room::calcNewOrigin(int wallid, VECTOR3D origin, Room * parent) {
  if(wallid == 1) return origin + (dir1v*parent->length);
  if(wallid == 2) return origin + (dir1v*parent->length) + (dir2v*parent->width);
  if(wallid == 3) return origin + (dir2v*parent->width);
  return origin;
}

// converts dir1v into an updated dir1v with appropriate rotation about axis up
// can only rotate wallid multiple of 90deg
VECTOR3D Room::newDir1(int wallid, VECTOR3D up) {
  int i;
  VECTOR3D result=dir1v;
  for(i=0; i<wallid; i++) {
    result = up.CrossProduct(result);
  }
  return result;
}

// Turns parent vectors into vectors for self
void Room::fixParentVectors(VECTOR3D * dir1v, VECTOR3D * dir2v, int pwall) {
  int i;
  VECTOR3D tmp;
  *dir2v *= -1;
  *dir1v *= -1;
  for(i=0;i<pwall;i++) {
    tmp = *dir2v;
    *dir2v = *dir1v * -1;
    *dir1v = tmp;
  }
  
}

// adds a neighbor
bool Room::addNeighbor(Room* newNeighbor, int wallid) {
  if(neighbor[wallid] != NULL) return false;
  neighbor[wallid] = newNeighbor;
  return true;
}

// pass along dir1v and dir2v of the wall, origin of the top left (from inside) corner of door (ie origin of panel[1]) door height(dh) and doorwidth (dw) and write doorframe data to QuadMesh * doorframe array
void Room::initDoorFrame(DoorWall * dw, VECTOR3D dir1v, VECTOR3D dir2v, VECTOR3D origin, float dh, float dwidth) {
  int i;
  //  dir2v *= -1;
  dir2v = dir1v.CrossProduct(dir2v);
  for(i=0; i<4; i++) {
    dw->doorframe[i] = new QuadMesh(MAX_MESH_SIZE);
    dw->doorframe[i]->InitMesh(MESH_SIZE, origin, DOOR_FRAME/2, (i%2==0? dwidth : dh), dir2v, dir1v);
    dw->tdoorframe[i] = makeTQ(origin, DOOR_FRAME/2, (i%2==0? dwidth : dh), dir2v, dir1v);
    origin += dir1v*(i%2==0? dwidth : dh);
    dir1v = dir1v.CrossProduct(dir2v);
  }
}

// Draw Texture 
void Room::drawTexture(TextureQuad * tq, GLuint texid, float mult1, float mult2) {  
  VECTOR3D origin = tq->origin;
  float len=tq->length*mult1;
  float wid=tq->width*mult2;

  // MUST BE CHANGED BACK TO GL_MODULATE 
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, texid);
  glBegin(GL_QUADS);

  glTexCoord2f( 0.0, 0.0);
  glVertex3f(origin.GetX(),origin.GetY(),origin.GetZ());

  origin += tq->dir2v*tq->width;
  glTexCoord2f( 0.0, wid);
  glVertex3f(origin.GetX(),origin.GetY(),origin.GetZ());

  origin += tq->dir1v*tq->length;
  glTexCoord2f( len, wid);
  glVertex3f(origin.GetX(),origin.GetY(),origin.GetZ());

  origin -= tq->dir2v*tq->width;
  glTexCoord2f( len, 0.0);
  glVertex3f(origin.GetX(),origin.GetY(),origin.GetZ());

  glEnd();
  glFlush();
  glPopMatrix();
}

TextureQuad * Room::makeTQ(VECTOR3D origin, float length, float width, VECTOR3D dir1v, VECTOR3D dir2v){
  TextureQuad *tq = (TextureQuad*) calloc (1, sizeof(TextureQuad));
  tq->origin = origin;
  tq->length = length;
  tq->width = width;
  tq->dir1v = dir1v;
  tq->dir2v = dir2v;
  return tq;
}

// Helper function for intersects
int withinRoom(VECTOR3D * minBB, VECTOR3D * maxBB, VECTOR3D * minRoom, VECTOR3D * maxRoom) {
  if(minBB->GetX() < minRoom->GetX()) return 3;
  //  if(minBB->GetY() < minRoom->GetY()) return false;
  if(minBB->GetZ() < minRoom->GetZ()) return 2;

  if(maxBB->GetX() > maxRoom->GetX()) return 1;
  //  if(maxBB->GetY() > maxRoom->GetY()) return false;
  if(maxBB->GetZ() > maxRoom->GetZ()) return 0;

  return -1;
}

// Move coordinates of minBB and maxBB by tx and tz
void moveBB(VECTOR3D * minBB, VECTOR3D * maxBB, float tx, float tz) {
  minBB->SetX(minBB->GetX() + tx);
  minBB->SetZ(minBB->GetZ() + tz);

  maxBB->SetX(maxBB->GetX() + tx);
  maxBB->SetZ(maxBB->GetZ() + tz);
}

bool Room::within_doorway(int wall_dir, int wall_id, VECTOR3D * minBB, VECTOR3D * maxBB){
  float dd = doorwall[wall_id]->dd;
  float dw = doorwall[wall_id]->dw;
  VECTOR3D origin1 = doorwall[wall_id]->origin[0];
  VECTOR3D origin2;
  VECTOR3D dir1v = doorwall[wall_id]->dir1v;

  origin1 += dir1v*(dd-dw/2);
  origin2 = origin1 + dir1v*dw;

  if(wall_dir == 0) {
    if(minBB->GetX() < origin1.GetX()) return false;
    if(maxBB->GetX() > origin2.GetX()) return false;
  }
  if(wall_dir == 1) {
    if(minBB->GetZ() > origin1.GetZ()) return false;
    if(maxBB->GetZ() < origin2.GetZ()) return false;
  }
  if(wall_dir == 2) {
    if(minBB->GetX() > origin1.GetX()) return false;
    if(maxBB->GetX() < origin2.GetX()) return false;
  }
  if(wall_dir == 3) {
    if(minBB->GetZ() < origin1.GetZ()) return false;
    if(maxBB->GetZ() > origin2.GetZ()) return false;
  }

  return true;
}
bool into_next_room(VECTOR3D pos, VECTOR3D * minRoom, VECTOR3D * maxRoom, int w) {
  if(w == 0)
    if(pos.GetZ() >= (maxRoom->GetZ() + DOOR_FRAME*2)) return true;
  if(w == 2)
    if(pos.GetZ() <= (minRoom->GetZ() - DOOR_FRAME*2)) return true;
  if(w == 1)
    if(pos.GetX() >= (maxRoom->GetX() + DOOR_FRAME*2)) return true;
  if(w == 3)
    if(pos.GetX() <= (minRoom->GetX() - DOOR_FRAME*2)) return true;

  return false;
}

// Check if tx and tz displacement will cause o to be in a wall
bool Room::intersects(Object * o, float tx, float tz) {
  VECTOR3D minBB, maxBB;
  VECTOR3D minRoom, maxRoom;
  int wall_id;
  int wall_dir;

  // Get o's and room's bounding box
  o->getBB(&minBB, &maxBB);
  moveBB(&minBB,&maxBB, tx, tz);
  wall_id = getRoomBB(&minRoom, &maxRoom);

  // Check if o is within main room boundaries
  // if yes, return false.
  wall_dir = withinRoom(&minBB,&maxBB,&minRoom,&maxRoom);
  if (wall_dir==-1) return false;
  
  // Calculate wall_id o is running into
  wall_id = (wall_id + wall_dir)%4;  

  // cout << wall_id << " " << neighbor[wall_id] << endl;

  // if the wall doesn't have a door, return true
  if(neighbor[wall_id] == NULL) return true;
  // else wall has a door, check if it's within the doorway
  else {
    if(within_doorway(wall_dir, wall_id, &minBB, &maxBB)) {
      // if yes check if o is past doorframe into next room,
      if(into_next_room(o->getPos(), &minRoom, &maxRoom, wall_dir)) {
	// if yes, update os room pointer to next room
	o->setCurrentRoom(neighbor[wall_id]);
	return false;
      }
      return false;
    }
  }

  // change to return true later
  return true;  
}


VECTOR3D Room::getCenter(void) {
  VECTOR3D center;
  center = origin;
  center += dir1v*(length/2);
  center += dir2v*(width/2);
  return center;
}

// Returns wall at Max z position
int Room::getRoomBB(VECTOR3D * minRoom, VECTOR3D * maxRoom) {
  if(dir1v.GetX() == 1){
    *minRoom = origin + dir2v*width;
    *maxRoom = origin + dir1v*length;
    return 0;
  } else if(dir1v.GetX() == -1) {
    *minRoom = origin + dir1v*length;
    *maxRoom = origin + dir2v*width;
    return 2;
  } else if(dir2v.GetX() == 1) {
    *minRoom = origin;
    *maxRoom = origin + dir1v*length + dir2v*width;
    return 1;
  } else {
    *minRoom = origin + dir1v*length + dir2v*width;
    *maxRoom = origin;
    return 3;
  }
}

bool Room::hitbot(Bullet * bul) {  
  for(list<EvilRobot*>::iterator it=bots.begin(); it!=bots.end(); ++it)
    if((*it)->hit(bul)) {
      delete (*it);
      bots.remove(*it);
      return true;
    }

  return false;
}  

// create new robot
void Room::newBot() {
  EvilRobot * bot;
  bot = new EvilRobot();
  bot->initRobot(this);
  bots.push_back(bot);
}

// Moves all the evil robots around the room they are in
void Room::move() {
  int dir;
  bool left, right;

  // Move evil robots
  for(list<EvilRobot*>:: iterator it=bots.begin(); it!=bots.end(); ++it) {
    left = right = false;
    srand(time(NULL));
    
    dir = rand() % 2;
    dir == 1 ? left = true : right = true;

    collision(*it);

    // While bot can't move forward, turn it left or right randomly
    while(!(*it)->move(true, false, false, false))
      (*it)->move(false, false, left, right);
  }
}

// Checks if any bots are colliding and if so changes their trajectories
void Room::collision(EvilRobot * bot) {
  for(list<EvilRobot*>:: iterator it=bots.begin(); it!=bots.end(); ++it) {
    if((*it) != bot && (*it)->hit(bot)) {
      (*it)->reverse();
      bot->reverse();
    }
  }
}