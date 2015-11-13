#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "VECTOR3D.h"
#include "QuadMesh.h"
#include "Room.hpp"

#define MESH_SIZE 16

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
    }
  }
}

bool Room::initRoom(float newLength, float newWidth, float newHeight) {
  int i;
  height = newHeight;
  length = newLength;
  width = newWidth;

  // If this room is the head of the roomtree, set values accordingly
  if(parent == NULL) {
    dir1v = VECTOR3D(1.0f, 0.0f, 0.0f);
    dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
    origin = VECTOR3D(0.0f, 0.0f, 0.0f);
  }
  else {
    // set room vectors to parent and fix based on parents wall id
    dir1v = parent->dir1v;
    dir2v = parent->dir2v;
    origin = calcNewOrigin((parent_wall+1)%4, parent->origin);
    fixParentVectors(&dir1v,&dir2v,parent_wall);

    // fetch origin from parent and calc adjusted one based on wall of parent

  }

  floor = new QuadMesh(1.0,1.0);
  floor->InitMesh(1.0, origin, length, width, dir1v, dir2v);
  
  for(i=0;i<4;i++)
    addDoor(i,-1,-1,-1);

  return true;
}

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
    dw->origin[1] = dw->origin[0] + dw->dir1v*dd + dw->dir2v*dh;
    dw->origin[2] = dw->origin[0] + dw->dir1v*(dd+dwidth);

    dw->len[0] = dd;
    dw->len[1] = dwidth;
    dw->len[2] = dw->length-dd-dwidth;
    dw->hgt[0] = height;
    dw->hgt[1] = height-dh;
    dw->hgt[2] = height;
  }


  // If this wall is supposed to have a door, create 3 new quadmeshes based on dimensions
  if(dd != -1) {
    for(i=0;i<3;i++) {
      dw->section[i] = new QuadMesh(1.0,1.0);
      dw->section[i]->InitMesh(1.0, dw->origin[i], dw->len[i], dw->hgt[i], dw->dir1v,dw->dir2v);
    }
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

VECTOR3D Room::newDir1(int wallid, VECTOR3D up) {
  int i;
  VECTOR3D result=dir1v;
  for(i=0; i<wallid; i++) {
    result = up.CrossProduct(result);
  }
  return result;
}

void Room::fixParentVectors(VECTOR3D * dir1v, VECTOR3D * dir2v, int pwall) {
  int i;
  VECTOR3D tmp;
  *dir2v *= -1;
  *dir1v *= -1;
  cout<<dir1v->GetX()<<dir1v->GetY()<<dir1v->GetZ()<<endl;
  cout<<dir2v->GetX()<<dir2v->GetY()<<dir2v->GetZ()<<endl;
  for(i=0;i<pwall;i++) {
    tmp = *dir2v;
    *dir2v = *dir1v * -1;
    *dir1v = tmp;
  }
  
}
