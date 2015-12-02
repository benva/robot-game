#ifndef EVILROBOT_H
#define EVILROBOT_H

#include "Robot.hpp"

using namespace std;

class Avatar;
class EvilRobot : public Robot {
private:
  void collision();

public:

  ~EvilRobot(){
    cout << "vaporizing bot" << endl;
  }

  void move();
  void attack(Avatar * avatar);
};

#endif
