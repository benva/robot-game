#ifndef GAME_H
#define GAME_H

#define WIDTH 1000
#define HEIGHT 600
#define FRAME_RATE 10
#define NUM_TEX 16
#define NUM_ROOMS 16
#define NUM_BUL 16

void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);

void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void mousePassiveHandler(int xMouse, int yMouse);

void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
void functionKeysUp (int key, int x, int y);

void tick(int value);

void setTexture(RGBpixmap *tex, GLuint textureID);
bool loadTexture(int i, char const * path);

void traverseRooms(Room * fromRoom, Room * aRoom, int n);
void updateRooms(Room * start);

Room * getRoomAt(int n);

VECTOR3D screenToWorld(void);
VECTOR3D mouseWorld;
VECTOR3D camera = VECTOR3D(0.0,6.0,22.0);
VECTOR3D dir = VECTOR3D(1.0,0.0,0.0);
VECTOR3D lookAt = VECTOR3D(0.0,0.0,0.0);
VECTOR3D room_center;

GLfloat light_position0[] = {3.0, 30.0, 3.0,0.0};
GLfloat light_position1[] = { 6.0,  12.0, 0.0,1.0};
//GLfloat light_diffuse[]   = {0.5, 0.5, 0.5, 1.0};
//GLfloat light_specular[]  = {0.5, 0.5, 0.5, 1.0};
GLfloat spot_direction[] = {0.0, -1.0, 0.0};
GLfloat light_diffuse[]   = {1.0, 1.0, 1.0, 1.0};
GLfloat light_specular[]  = {1.0, 1.0, 1.0, 1.0};

GLfloat light_ambient[]   = {0.0, 0.0, 0.0, 1.0};

float camX=0.0, camY=6.0, camZ=22.0, camR=30.0,
  lookAtX=0.0, lookAtY=0.0, lookAtZ=0.0,
  camTheta, camPhi;

RGBpixmap * tex[NUM_TEX];
GLuint texid[NUM_TEX];

float meshSize = 16.0;

Avatar * avatar;
//EvilRobot * bot;
//Bullet * bullet;

list<Bullet*> bullets;
list<Room*> rooms;
//list<EvilRobot*> bots;

//OBJModel * robotModel;

// bools for arrow keys
bool key_up = false;
bool key_down = false;
bool key_right = false;
bool key_left = false;

int mouse_x, mouse_y;

bool above_view=false;
#endif
