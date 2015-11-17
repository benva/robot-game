#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "RGBpixmap.h"
#include "VECTOR3D.h"
#include "QuadMesh.h"
#include "Robot.hpp"
#include "Room.hpp"

using namespace std;

void Room::draw() {
  int i,j;

  drawTexture(tfloor, floor_texture,0.5,0.5);

  floor->DrawMesh(1.0);

  for(i=0;i<4;i++) {
    if(doorwall[i]->dd == -1){
      drawTexture(doorwall[i]->tsection[0], wall_texture,1,1);
      //      doorwall[i]->section[0]->DrawMesh(1.0);
    } else {
      for(j=0; j<3; j++) {
	drawTexture(doorwall[i]->tsection[j], wall_texture,1,1);
	//doorwall[i]->section[j]->DrawMesh(1.0);
      }
      for(j=0; j<4; j++) {
	drawTexture(doorwall[i]->tdoorframe[j], door_texture,1,1);
	//	doorwall[i]->doorframe[j]->DrawMesh(1.0);
      }
    }
  }
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
    origin = calcNewOrigin((parent_wall+1)%4, neighbor[0]->origin);
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

  floor = new QuadMesh(1.0,1.0);
  floor->InitMesh(1.0, origin, length, width, dir1v, dir2v);
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
      dw->section[i] = new QuadMesh(1.0,1.0);
      dw->section[i]->InitMesh(1.0, dw->origin[i], dw->len[i], dw->hgt[i], dw->dir1v,dw->dir2v);
      dw->tsection[i] = makeTQ(dw->origin[i], dw->len[i], dw->hgt[i], dw->dir1v, dw->dir2v);
    }
    // Create doorframe meshes
    initDoorFrame(dw, dw->dir1v, dw->dir2v, dw->origin[1], dw->dh, dw->dw);
    // If this is a doorless wall, create just the first quadmesh
  } else {
    dw->section[0] = new QuadMesh(1.0,1.0);
    dw->section[0]->InitMesh(1.0, dw->origin[0], dw->length, height, dw->dir1v,dw->dir2v);
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
    dw->doorframe[i] = new QuadMesh(1.0,1.0);
    dw->doorframe[i]->InitMesh(1.0, origin, DOOR_FRAME/2, (i%2==0? dwidth : dh), dir2v, dir1v);
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
bool withinRoom(VECTOR3D * minBB, VECTOR3D * maxBB, VECTOR3D * minRoom, VECTOR3D * maxRoom) {
  if(minBB->GetX() < minRoom->GetX()) return false;
  //  if(minBB->GetY() < minRoom->GetY()) return false;
  if(minBB->GetZ() < minRoom->GetZ()) return false;

  if(maxBB->GetX() > maxRoom->GetX()) return false;
  //  if(maxBB->GetY() > maxRoom->GetY()) return false;
  if(maxBB->GetZ() > maxRoom->GetZ()) return false;

  return true;
}

// Move coordinates of minBB and maxBB by tx and tz
void moveBB(VECTOR3D * minBB, VECTOR3D * maxBB, float tx, float tz) {
  minBB->SetX(minBB->GetX() + tx);
  minBB->SetZ(minBB->GetZ() + tz);

  maxBB->SetX(maxBB->GetX() + tx);
  maxBB->SetZ(maxBB->GetZ() + tz);
}

// Check if tx and tz displacement will cause rob to be in a wall
bool Room::intersects(Robot * rob, float tx, float tz) {
  VECTOR3D minBB, maxBB;
  VECTOR3D minRoom, maxRoom;
  // Get rob's and room's bounding box
  rob->getBB(&minBB, &maxBB);
  moveBB(&minBB,&maxBB, tx, tz);
  getRoomBB(&minRoom, &maxRoom);

  cout << "MinRoom: " << minRoom.GetX() << " " << minRoom.GetY() << " " << minRoom.GetZ() << endl;
  cout << "MaxRoom: " << maxRoom.GetX() << " " << maxRoom.GetY() << " " << maxRoom.GetZ() << endl<<endl;
  cout << "MinBB: " << minBB.GetX() << " " << minBB.GetY() << " " << minBB.GetZ() << endl;
  cout << "MaxBB: " << maxBB.GetX() << " " << maxBB.GetY() << " " << maxBB.GetZ() << endl<<endl;

  // Check if rob is within main room boundaries
  // if yes, return false.
  if (withinRoom(&minBB,&maxBB,&minRoom,&maxRoom)) return false;
  // else check if rob is within doorway on intersecting wall
  // if yes check if rob is past doorframe into next room,
  // if yes, update robs room pointer to next room
  // and return false

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

void Room::getRoomBB(VECTOR3D * minRoom, VECTOR3D * maxRoom) {
  if(dir1v.GetX() == 1){ // GUD
    *minRoom = origin + dir2v*width;
    *maxRoom = origin + dir1v*length;
  } else if(dir1v.GetX() == -1) {
    *minRoom = origin + dir1v*length;
    *maxRoom = origin + dir2v*width;
  } else if(dir2v.GetX() == 1) { //GUD
    *minRoom = origin;
    *maxRoom = origin + dir1v*length + dir2v*width;
  } else {
    *minRoom = origin + dir1v*length + dir2v*width;
    *maxRoom = origin;
  }
}
