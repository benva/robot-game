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
  int i;
  floor->DrawMesh(length*width);

  for(i=0;i<4;i++)
    doorwall[i]->section[0]->DrawMesh(doorwall[i]->size[0]);
}

bool Room::initRoom(float newHeight, float newLength, float newWidth) {
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
  return true;
}

bool Room::addDoor(int wallid, float dw, float dd) {
  VECTOR3D sec1origin=origin, sec2origin=origin;
  int i;
  if(doorwall[wallid] != NULL) return false;
  
  doorwall[wallid] = (DoorWall*) calloc (1, sizeof(DoorWall));
  doorwall[wallid]->dd = dd;
  doorwall[wallid]->dw = dw;
  doorwall[wallid]->origin = calcNewOrigin(wallid);
  doorwall[wallid]->dir2v = dir1v.CrossProduct(dir2v);
  doorwall[wallid]->dir1v = newDir1(wallid, doorwall[wallid]->dir2v);

  cout << wallid << endl;
  cout << "dir1v: " << doorwall[wallid]->dir1v.GetX() << " " << doorwall[wallid]->dir1v.GetY() << " " << doorwall[wallid]->dir1v.GetZ()<< endl;

  cout << "dir2v: " <<doorwall[wallid]->dir2v.GetX() << " " << doorwall[wallid]->dir2v.GetY() << " " << doorwall[wallid]->dir2v.GetZ()<< endl << endl;


  if(wallid%2 == 0) {
    for(i=0;i<3;i++){
      doorwall[wallid]->size[i] = length*height;
      doorwall[wallid]->section[i] = new QuadMesh(length*height);
    }
    doorwall[wallid]->section[0]->InitMesh(length*height, doorwall[wallid]->origin, length, height, doorwall[wallid]->dir1v, doorwall[wallid]->dir2v);
  }
  else {
    for(i=0;i<3;i++){
      doorwall[wallid]->size[i] = width*height;
      doorwall[wallid]->section[i] = new QuadMesh(width*height);
    }
    doorwall[wallid]->section[0]->InitMesh(width*height, doorwall[wallid]->origin, width, height, doorwall[wallid]->dir1v, doorwall[wallid]->dir2v);
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
