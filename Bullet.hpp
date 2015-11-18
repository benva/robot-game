#ifndef BULLET_H
#define BULLET_H

#define BUL_INC 5

#include "Object.hpp"
#include "Robot.hpp"

class Bullet : public Object {
private:
  GLUquadric * q;
public:
	Bullet(Robot * r) {
    dir = r->getDir();
		position = r->getPos();
    q = gluNewQuadric();
	}

  ~Bullet() {
    // std::cout << "Deleting bullet" << std::endl;
  }

	void draw(GLuint texid);

	void move();
};

#endif