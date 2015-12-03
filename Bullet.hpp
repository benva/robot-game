#ifndef BULLET_H
#define BULLET_H

#define BUL_INC 5
#include <list>
#include "Object.hpp"

using namespace std;

class Robot;
class Bullet : public Object {
private:
  GLUquadric * q;
public:
  Bullet(Robot * r);

  ~Bullet() {
    cout << "deleting bullet" << endl;
  }

	void draw(GLuint texid);

	bool move();
};


#endif
