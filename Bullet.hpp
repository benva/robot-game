#ifndef BULLET_H
#define BULLET_H

class Bullet {
private:
	float angle;
	VECTOR3D dir;
	VECTOR3D position;
  GLUquadric * q;
public:
	Bullet(Robot * r) {
    dir = r->getDir();
		position = r->getPos();
    q = gluNewQuadric();
	}

	void draw(GLuint texid);

	void move();

  VECTOR3D getPos(void) {return position;}
};

#endif