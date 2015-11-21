#ifndef EVILROBOT_H
#define EVILROBOT_H

#include "Robot.hpp"

using namespace std;

class EvilRobot : public Robot {
private:

public:

  ~EvilRobot(){
    cout << "vaporizing bot" << endl;
  }
};

#endif
