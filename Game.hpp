#ifndef GAME_H
#define GAME_H

#define WIDTH 1000
#define HEIGHT 800
#define FRAME_RATE 50
#define NUM_TEX 16
#define NUM_ROOMS 16

void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void tick(int value);
void keyUp (int, int, int);
void setTexture(RGBpixmap *tex, GLuint textureID);
bool loadTexture(int i, char path[]);

GLfloat light_position0[] = {-6.0,  12.0, 0.0,1.0};
GLfloat light_position1[] = { 6.0,  12.0, 0.0,1.0};
GLfloat light_diffuse[]   = {1.0, 1.0, 1.0, 1.0};
GLfloat light_specular[]  = {1.0, 1.0, 1.0, 1.0};
GLfloat light_ambient[]   = {0.2, 0.2, 0.2, 1.0};

float camX=0.0, camY=6.0, camZ=22.0, camR=10.0,
  lookAtX=0.0, lookAtY=0.0, lookAtZ=0.0,
  camTheta, camPhi;


Room *room[NUM_ROOMS];

RGBpixmap * tex[NUM_TEX];
GLuint texid[NUM_TEX];

float meshSize = 16.0;
Robot *r = new Robot();

#endif
