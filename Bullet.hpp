#ifndef BULLET_H
#define BULLET_H

#define BUL_INC 5
#include <list>
#include "Object.hpp"

using namespace std;

class Robot;
class Avatar;
class Bullet : public Object {
private:
    GLUquadric * q;
    Robot * me;
public:
    Bullet(Robot * r);

    ~Bullet() {

    }

    void draw(GLuint texid);

    bool move(Avatar * avatar);
};


#endif
