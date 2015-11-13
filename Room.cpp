#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "VECTOR3D.h"
#include "QuadMesh.h"
#include "Room.hpp"



using namespace std;

void Room::draw() {
  int i,j;

  floor->DrawMesh(1.0);

  for(i=0;i<4;i++) {
    if(doorwall[i]->dd == -1){
      doorwall[i]->section[0]->DrawMesh(1.0);
    } else {
      for(j=0; j<3; j++) 
	doorwall[i]->section[j]->DrawMesh(1.0);
      for(j=0; j<4; j++)
      doorwall[i]->doorframe[j]->DrawMesh(1.0);
    }
  }
}

bool Room::initRoom(float newLength, float newWidth, float newHeight) {
  int i;
  height = newHeight;
  length = newLength;
  width = newWidth;



  // If this room is the head of the roomtree, set values accordingly
  if(neighbor[0] == NULL) {
    dir1v = VECTOR3D(1.0f, 0.0f, 0.0f);
    dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
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
    }
    // Create doorframe meshes
    initDoorFrame(dw->doorframe, dw->dir1v, dw->dir2v, dw->origin[1], dw->dh, dw->dw);
    // If this is a doorless wall, create just the first quadmesh
  } else {
    dw->section[0] = new QuadMesh(1.0,1.0);
    dw->section[0]->InitMesh(1.0, dw->origin[0], dw->length, height, dw->dir1v,dw->dir2v);
  }    

  return true;
}

// returns origin of each corner based on the wall id
VECTOR3D Room::calcNewOrigin(int wallid, VECTOR3D origin) {
  if(wallid == 0) return origin;
  if(wallid == 1) return origin + (dir1v*length);
  if(wallid == 2) return origin + (dir1v*length) + (dir2v*width);
  if(wallid == 3) return origin + (dir2v*width);
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
void Room::initDoorFrame(QuadMesh * doorframe[4], VECTOR3D dir1v, VECTOR3D dir2v, VECTOR3D origin, float dh, float dw) {
  int i;
  //  dir2v *= -1;
  dir2v = dir1v.CrossProduct(dir2v);
  for(i=0; i<4; i++) {
    doorframe[i] = new QuadMesh(1.0,1.0);
    doorframe[i]->InitMesh(1.0, origin, DOOR_FRAME/2, (i%2==0? dw : dh), dir2v, dir1v);
    origin += dir1v*(i%2==0? dw : dh);
    dir1v = dir1v.CrossProduct(dir2v);
  }
}
