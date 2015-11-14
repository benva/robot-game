#include "Robot.hpp"


void Robot::draw(GLuint texid) {
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glPushMatrix();

  glBindTexture(GL_TEXTURE_2D, texid);
  glBegin(GL_QUADS);
  glTexCoord2f( 0.0, 0.0);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glTexCoord2f( 0.0, 1.0);  
  glVertex3f(-1.0f, 1.0f,  1.0f);
  glTexCoord2f( 1.0, 1.0);
  glVertex3f( 1.0f, 1.0f,  1.0f);
  glTexCoord2f( 1.0, 0.0);
  glVertex3f( 1.0f, 1.0f, -1.0f);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f( 0.0, 0.0);
  glVertex3f( 1.0f, 1.0f, -1.0f);
  glTexCoord2f( 0.0, 1.0);
  glVertex3f( 1.0f, 1.0f,  1.0f);
  glTexCoord2f( 1.0, 1.0);
  glVertex3f( 1.0f, -1.0f,  1.0f);
  glTexCoord2f( 1.0, 0.0);
  glVertex3f( 1.0f, -1.0f, -1.0f);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f( 0.0, 0.0);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glTexCoord2f( 0.0, 1.0);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glTexCoord2f( 1.0, 1.0);
  glVertex3f(-1.0f, -1.0f,  1.0f);
  glTexCoord2f( 1.0, 0.0);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f( 0.0, 0.0);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glTexCoord2f( 0.0, 1.0);
  glVertex3f(-1.0f, -1.0f,  1.0f);
  glTexCoord2f( 1.0, 1.0);
  glVertex3f( 1.0f, -1.0f,  1.0f);
  glTexCoord2f( 1.0, 0.0);
  glVertex3f( 1.0f, -1.0f, -1.0f);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f( 0.0, 0.0);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glTexCoord2f( 0.0, 1.0);
  glVertex3f( -1.0f, 1.0f,  -1.0f);
  glTexCoord2f( 1.0, 1.0);
  glVertex3f( 1.0f, 1.0f,  -1.0f);
  glTexCoord2f( 1.0, 0.0);
  glVertex3f( 1.0f, -1.0f, -1.0f);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f( 0.0, 0.0);
  glVertex3f(-1.0f, -1.0f, 1.0f);
  glTexCoord2f( 0.0, 1.0);
  glVertex3f(-1.0f, 1.0f,  1.0f);
  glTexCoord2f( 1.0, 1.0);
  glVertex3f( 1.0f, 1.0f,  1.0f);
  glTexCoord2f( 1.0, 0.0);
  glVertex3f( 1.0f, -1.0f, 1.0f);
  glEnd();

  glPopMatrix();
}

void Robot::moveForward(float d) {

}

void Robot::moveBackward(float d) {

}

void Robot::rotate(float angle) {

}
