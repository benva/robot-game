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
#include <list>

#include "RGBpixmap.h"


#include "VECTOR3D.h"
#include "QuadMesh.h"
#include "Object.hpp"
#include "Room.hpp"

#include "OBJModel.hpp"
#include "Robot.hpp"

#include "Avatar.hpp"
#include "Bullet.hpp"
#include "EvilRobot.hpp"

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
  glutKeyboardFunc(keyboard);
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
 Room * room;
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0,1.0,0.2,80.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

    // Set up and enable lighting
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  
  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  //  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45);
  //  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);

  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);


  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  //  glEnable(GL_LIGHT1);

  //  glEnable(GL_NORMALIZE);


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
  
  //wall textures
  loadTexture(0, "textures/brick1.bmp");
  loadTexture(1, "textures/brick2.bmp");
  loadTexture(2, "textures/stone2.bmp");
  loadTexture(3, "textures/stone3.bmp");

  //floor textures
  loadTexture(4, "textures/stone1.bmp");
  loadTexture(5, "textures/rock.bmp");
  loadTexture(6, "textures/wood.bmp");

  //robot texture
  loadTexture(7, "textures/evil_robot.bmp");
  
  i=-1;
  while(tex[++i] != NULL){
    setTexture(tex[i], texid[i]);
  }

  // in future use single room pointer and initialize new rooms only for that
  // then add them to the room set with insert

  room = new Room();
  room->initRoom();
  rooms.push_back(room);
  
  room = new Room(getRoomAt(0),1);
  room->initRoom(14,15);
  room->setTextures(texid[5],texid[6]);
  rooms.push_back(room);

  room = new Room(getRoomAt(0),3);
  room->initRoom(12,8);
  room->setTextures(texid[2],texid[3]);
  rooms.push_back(room);
  
  room = new Room(getRoomAt(2),3);
  room->initRoom(6,6);
  room->setTextures(texid[4],texid[0]);
  rooms.push_back(room);
  
  avatar = new Avatar();
  avatar->initRobot(rooms.front());
  

  Robot::model.load("models/evil_robot.obj");
}

Room * getRoomAt(int n) {
  list<Room*>::iterator it = rooms.begin();
  for(int i=0; i<n; ++it, ++i)
    if(it == rooms.end()) return NULL;    

  return (*it);
}

// Helper function for update
bool noNeighbors(Room * fromRoom, Room * aRoom) {
  int i;
  Room * neighbor;
  for(i=0; i<4; i++) {
    neighbor = aRoom->getNeighbor(i);
    if(neighbor != NULL && neighbor != fromRoom )
      return false;
  }
  return true;
}

void updateRooms(Room * start) {
  int i;
  Room * neighbor;
  for(i=0;i<4;i++) {
    neighbor = start->getNeighbor(i);
    if(neighbor != NULL) {
      cout << "traverse " << neighbor << endl;
      traverseRooms(start, neighbor ,1);

    }
  }
  return;
}

// Update rooms based on players movement from fromRoom into aRoom to a recursive depth of n
void traverseRooms(Room * fromRoom, Room * aRoom, int n) {
  int i;
  Room * neighbor;
  if( n == 0 ) {
    for(i=0;i<4;i++) {
      neighbor = aRoom->getNeighbor(i);
      if(neighbor != NULL && neighbor != fromRoom) {
	cout << "remove " << neighbor << endl;
	rooms.remove(neighbor);
	delete neighbor;
      }
    }
    return;
  }
  else if( n == 1 && noNeighbors(fromRoom, aRoom)) {
    cout << "make new rooms at " << aRoom << endl;
    srand(time(NULL));
    for(i=0; i<4; i++) {
      neighbor = aRoom->getNeighbor(i);
      if(neighbor != fromRoom && neighbor == NULL) {	
	if(rand()%3 == 0) {
	  neighbor = new Room(aRoom,i);
	  neighbor->initRoom(6,6);
	  neighbor->setTextures(texid[rand()%4],texid[rand()%3+4]);
	  rooms.push_back(neighbor);
	}
      }
    }
    // create 1-3 new neighbors randomly    
    return;
  }
  else {
    n--;
    for(i=0; i<4; i++) {
      neighbor = aRoom->getNeighbor(i);
      if(neighbor != fromRoom && neighbor != NULL){
	cout << "traverse from " << aRoom << " to " << neighbor << endl;
	traverseRooms(aRoom, neighbor, n);
      }
    }
  }
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
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // Set up camera
  gluLookAt(camX, camY, camZ, lookAtX, lookAtY, lookAtZ, 0.0,1.0,0.0);

  // ITERATE through room set to draw meshes - in turn draws all bots
  for(list<Room*>::iterator it=rooms.begin(); it!=rooms.end(); ++it)
    (*it)->draw();

  //Draw avatar
  avatar->draw(texid[0]);
  
  //Draw Bullets
  for(list<Bullet*>::iterator it=bullets.begin(); it!=bullets.end(); ++it)
    (*it)->draw(texid[6]);

  //  mod->draw();
  glutSwapBuffers();
}

void reshape(int w, int h) {
  cout << "W: " << w << " H: " << h << endl;
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, 1.0, 0.2, 40.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void tick(int value) {
  // Mode B: auto room generation
  /*  
  current = avatar->getCurrentRoom();
   avatar->move(key_up, key_down, key_left, key_right);
  if(current != avatar->getCurrentRoom())
    updateRooms(avatar->getCurrentRoom());
  */
  Room * current;
  // Mode A: static rooms
  // Update Avatar position
  avatar->move(key_up, key_down, key_left, key_right);

  // Move all Bullets
  for(list<Bullet*>::iterator it=bullets.begin(); it!=bullets.end();)
    if(!(*it)->move()) {
      list<Bullet*>::iterator del=it;
      ++it;
      delete (*del);      
      bullets.remove(*del);
    } else ++it;

  // Call update function in room
  // If avatar is in room, attack them, otherwise move bots around room
  current = avatar->getCurrentRoom();
  for(list<Room*>::iterator it=rooms.begin(); it!=rooms.end(); ++it) {
    if((*it) == current)
      (*it)->attack(avatar);
    else (*it)->move();
  }

  /*  if(bullet && !bullet->move()){ 
    delete bullet; 
    bullet = NULL; 
    }*/

  /*  if(bullet && bot && bot->hit(bullet)) { 
    delete bot; 
    bot = NULL; 
    delete bullet;
    bullet = NULL;
    }*/
  
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
  room_center = avatar->getCurrentRoom()->getCenter();
  //  cout << room_center << endl;
  light_position0[0] = room_center.GetX();
  light_position0[2] = room_center.GetZ();
  
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

void keyboard(unsigned char key, int x, int y) {
  Bullet * bullet;
  // Make avatar shoot
  if(key == ' '){
    bullet = new Bullet(avatar);
    bullets.push_back(bullet);
  }
   
  // Create a bot
  //TAKE OUT
  if(key == 'b')
    rooms.back()->newBot();
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
