#ifndef EVILROBOT_H
#define EVILROBOT_H

#include "Robot.hpp"

using namespace std;

class Avatar;
class EvilRobot : public Robot {
private:
    void collision();
    int shootTimeout;
    int rotateTimeout;
    int forwardTimeout;
public:
    EvilRobot();
    ~EvilRobot(){

    }
    void move();
    Bullet* attack(Avatar * avatar);
};

#endif
