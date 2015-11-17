#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <utility>
#include <vector>

#include "RGBpixmap.h"

#include "VECTOR3D.h"
#include "QuadMesh.h"

#include "Room.hpp"
#include "Robot.hpp"

#include "Game.hpp"


using namespace std;

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(0,0);
  glutCreateWindow("Robot Game");

  initOpenGL(WIDTH,HEIGHT);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mouseMotionHandler);
  //glutPassiveMotionFunc(mousePassiveHandler);
  
  glutSpecialFunc(functionKeys);
  glutSpecialUpFunc(functionKeysUp);

  glutTimerFunc(FRAME_RATE, tick, 0);
  glutMainLoop();
  return 0;
}

// Setup openGL
void initOpenGL(int w, int h) {
int i;
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0,1.0,0.2,80.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

    // Set up and enable lighting
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  
  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  // Other OpenGL setup
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glClearColor(0.6, 0.6, 0.6, 0.0);  
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  
  // Probably good to enable
  glEnable(GL_CULL_FACE);

  //Enable textures
  glEnable(GL_TEXTURE_2D);

  // This one is important - renormalize normal vectors 
  glEnable(GL_NORMALIZE);

  // Hide Cursor
  glutSetCursor(GLUT_CURSOR_NONE);
  


  //Nice perspective.
  glHint(GL_PERSPECTIVE_CORRECTION_HINT , GL_NICEST);

  VECTOR3D ambient = VECTOR3D(0.0f,0.0f,0.0f);
  VECTOR3D specular= VECTOR3D(0.0f,0.0f,0.0f);
  VECTOR3D diffuse= VECTOR3D(0.9f,0.5f,0.0f);

  for(i=0; i<NUM_TEX; i++) {
    texid[i] = 2000+i; 
 }
  
  loadTexture(0, "textures/rock.bmp");
  loadTexture(1, "textures/brick1.bmp");
  loadTexture(2, "textures/brick2.bmp");
  loadTexture(3, "textures/stone1.bmp");
  loadTexture(4, "textures/stone2.bmp");
  loadTexture(5, "textures/stone3.bmp");
  loadTexture(6, "textures/wood.bmp");

  
  i=-1;
  while(tex[++i] != NULL){
    setTexture(tex[i], texid[i]);
  }

  room[0] = new Room();
  room[0]->initRoom();

  room[1] = new Room(room[0],1);
  room[1]->initRoom();
  room[1]->setTextures(texid[5],texid[6]);

  room[2] = new Room(room[1],1);
  room[2]->initRoom();
  room[2]->setTextures(texid[2],texid[3]);

  room[3] = new Room(room[1],3);
  room[3]->initRoom(6,6);
  room[3]->setTextures(texid[4],texid[0]);

avatar = new Robot();
avatar->initRobot(room[0]);
}

bool loadTexture(int i, char const * path) {
  tex[i] = new RGBpixmap;
  tex[i]->readBMPFile(path);
  return true;
}

void setTexture(RGBpixmap * tex, GLuint textureID) {
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->nCols, tex->nRows, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->pixel);
}

// function to display everything to the screen
void display(void) {
  int i;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // Set up camera
  gluLookAt(camX, camY, camZ, lookAtX, lookAtY, lookAtZ, 0.0,1.0,0.0);

  // Draw the Room meshes

  i=-1;
  while(room[++i] != NULL)
    room[i]->draw();

  //Draw the Enemy Robots
  //INSERT CODE
  avatar->draw(2000);

  //Draw avatar
  //INSERT CODE
  
  glutSwapBuffers();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, 1.0, 0.2, 40.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void tick(int value) {
  // Call Evil robot move methods

  // Update Avatar position
  avatar->move(key_up, key_down, key_left, key_right);
  
  camera = avatar->getPos();
  dir = avatar->getDir();
  lookAt = camera+dir;

  if(!above_view) {
    camX = avatar->getX();
    camY = avatar->getY();
    camZ = avatar->getZ();
    
    lookAtX = lookAt.GetX();
    lookAtY = lookAt.GetY();
    lookAtZ = lookAt.GetZ();
    
  }

  glutPostRedisplay();
  glutTimerFunc(FRAME_RATE, tick, 1);
}

//Updates camera coordinates
void updateCamera() {
  camZ = camR*sin(camTheta) * cos(camPhi) + avatar->getZ();
  camX = camR*sin(camTheta) * sin(camPhi) + avatar->getX();
  camY = camR*cos(camTheta);
}

float map(float value, float from, float to) {
  float output;
  output = (to/from)*value;
  return output;
}

void mouse(int button, int state, int x, int y) {
  if(state == GLUT_UP)
    if(button == GLUT_LEFT_BUTTON) {
      //      above_view=false;
      glutSetCursor(GLUT_CURSOR_NONE);
    }
}

// Mouse motion callback - use only if you want to 
void mouseMotionHandler(int xMouse, int yMouse)
{
  lookAtX = avatar->getX();
  lookAtY = avatar->getY();
  lookAtZ = avatar->getZ();

  glutSetCursor(GLUT_CURSOR_CROSSHAIR);
  camTheta = map(500-yMouse,500,90)+270;
  camPhi = map(xMouse,500,360);
  camTheta = (camTheta * M_PI)/180;
  camPhi = (camPhi * M_PI)/180;
  updateCamera();

  glutPostRedisplay();
}

// Updates global mouse coordinates passively
void mousePassiveHandler(int xMouse, int yMouse) {
  mouse_x = xMouse;
  mouse_y = yMouse;
  
  mouseWorld = screenToWorld();
  lookAtX=mouseWorld.GetX();
  lookAtY=mouseWorld.GetY();
  lookAtZ=mouseWorld.GetZ();
  //  cout << lookAtX << endl;
}

void functionKeys(int key, int x, int y) {
  if(key == GLUT_KEY_F1) {
    above_view == true? above_view = false: above_view = true;
    camX = 0.0; camY = 6.0; camZ = 10.0;
  }
  if(key == GLUT_KEY_UP) key_up = true;
  if(key == GLUT_KEY_DOWN) key_down = true;
  if(key == GLUT_KEY_RIGHT) key_right = true;
  if(key == GLUT_KEY_LEFT) key_left = true;
}

void functionKeysUp(int key, int x, int y) {
  if(key == GLUT_KEY_UP) key_up = false;
  if(key == GLUT_KEY_DOWN) key_down = false;
  if(key == GLUT_KEY_RIGHT) key_right = false;
  if(key == GLUT_KEY_LEFT) key_left = false;
}

VECTOR3D screenToWorld() {
  VECTOR3D coord;
  GLdouble projection[16];
  GLdouble modelview[16];
  GLint viewport[4];
  GLdouble pos[3];

  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  glGetIntegerv(GL_VIEWPORT, viewport);
  
  gluUnProject(mouse_x, mouse_y, 1.0f, modelview, projection, viewport, &pos[0], &pos[1], &pos[2]);
  
  coord = VECTOR3D(pos[0],pos[1],pos[2]);
  cout << coord.GetX() << " " << coord.GetY() << " " << coord.GetZ() << endl;
  return coord;
}
