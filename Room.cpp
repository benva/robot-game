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

  floor->DrawMesh(length*width);

  for(i=0;i<4;i++) {
    if(doorwall[i]->dd == -1){
      doorwall[i]->section[0]->DrawMesh(doorwall[i]->size[0]);
    } else {
      for(j=0; j<3; j++) 
	doorwall[i]->section[j]->DrawMesh(doorwall[i]->size[j]);
    }
  }
}

bool Room::initRoom(float newHeight, float newLength, float newWidth) {
  int i;
  height = newHeight;
  length = newLength;
  width = newWidth;
  
  if(parent == NULL) {
    dir1v = VECTOR3D(1.0f, 0.0f, 0.0f);
    dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
    origin = VECTOR3D(0.0f, 0.0f, 0.0f);
  }

  floor = new QuadMesh(length*width);
  floor->InitMesh(length*width, origin, length, width, dir1v, dir2v);
  
  for(i=0;i<4;i++)
    addDoor(i,-1,-1,-1);


  return true;
}

bool Room::addDoor(int wallid, float dwidth, float dd, float dh) {
  int i;
  doorwall[wallid] = (DoorWall*) calloc (1,sizeof(DoorWall));
  DoorWall * dw = doorwall[wallid];

  dw->length = wallid%2 == 0? length : width;
  dw->origin[0] = calcNewOrigin(wallid);
  dw->dir2v = dir1v.CrossProduct(dir2v);
  dw->dir1v = newDir1(wallid, dw->dir2v);
  dw->dd = dd;

  cout << wallid << endl;
  cout << "dir1v: " << dw->dir1v.GetX() << " " << dw->dir1v.GetY() << " " << dw->dir1v.GetZ()<< endl;

  cout << "dir2v: " <<dw->dir2v.GetX() << " " << dw->dir2v.GetY() << " " << dw->dir2v.GetZ()<< endl << endl;

  dw->origin[1] = dw->origin[0] + dw->dir1v*dd + dw->dir2v*dh;
  dw->origin[2] = dw->origin[0] + dw->dir1v*(dd+dwidth);
  
  if(dd != -1) {
    dw->len[0] = dd;
    dw->len[1] = dwidth;
    dw->len[2] = dw->length-dd-dwidth;
    dw->hgt[0] = height;
    dw->hgt[1] = height-dh;
    dw->hgt[2] = height;
  }


  if(dd != -1) {
    for(i=0;i<3;i++) {
      dw->size[i] = dw->len[i]*dw->hgt[i];
      dw->section[i] = new QuadMesh(dw->size[i]);
      dw->section[i]->InitMesh(dw->size[i], dw->origin[i], dw->len[i], dw->hgt[i], dw->dir1v,dw->dir2v);
    }
  } else {
    dw->size[0] = dw->length*height;
    dw->section[0] = new QuadMesh(dw->size[0]);
    dw->section[0]->InitMesh(dw->size[0], dw->origin[0], dw->length, height, dw->dir1v,dw->dir2v);
  }    

return true;
}

VECTOR3D Room::calcNewOrigin(int wallid) {
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
