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

#include "VECTOR3D.h"
#include "QuadMesh.h"
#include "Robot.hpp"
#include "Room.hpp"
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
glutMotionFunc(mouseMotionHandler);

  glutTimerFunc(FRAME_RATE, tick, 0);
  glutMainLoop();
  return 0;
}

// Setup openGL
void initOpenGL(int w, int h) {
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
  
  // Other OpenGL setup
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glClearColor(0.6, 0.6, 0.6, 0.0);  
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  // This one is important - renormalize normal vectors 
  glEnable(GL_NORMALIZE);
  
  //Nice perspective.
  glHint(GL_PERSPECTIVE_CORRECTION_HINT , GL_NICEST);

  VECTOR3D ambient = VECTOR3D(0.0f,0.0f,0.0f);
  VECTOR3D specular= VECTOR3D(0.0f,0.0f,0.0f);
  VECTOR3D diffuse= VECTOR3D(0.9f,0.5f,0.0f);
  float shininess = 0.0;
  
room[0] = new Room();
room[0]->initRoom(2,4,4);
room[0]->addDoor(0,8,2);
room[0]->addDoor(1,8,2);
room[0]->addDoor(2,8,2);
room[0]->addDoor(3,8,2);
  room[0]->draw();
  
}

// function to display everything to the screen
void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // Set up camera
  gluLookAt(camX, camY, camZ, lookAtX, lookAtY, lookAtZ, 0.0,1.0,0.0);

  // Draw the Room meshes
  //INSERT CODE
  room[0]->draw();

  //Draw the Enemy Robots
  //INSERT CODE

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

/*  cout << r->getX() << endl;
  cout << r->getHealth() << endl;
  r->draw();
*/

  glutPostRedisplay();
  glutTimerFunc(FRAME_RATE, tick, 1);
}

//Updates camera coordinates
void updateCamera() {
      camZ = camR*sin(camTheta) * cos(camPhi);
      camX = camR*sin(camTheta) * sin(camPhi);
      camY = camR*cos(camTheta);
}

float map(float value, float from, float to) {
  float output;
  output = (to/from)*value;
  return output;
}


// Mouse motion callback - use only if you want to 
void mouseMotionHandler(int xMouse, int yMouse)
{
camTheta = map(500-yMouse,500,90)+270;
camPhi = map(xMouse,500,360);
camTheta = (camTheta * M_PI)/180;
camPhi = (camPhi * M_PI)/180;
updateCamera();
 
  glutPostRedisplay();
}
