#ifndef SIMPLEPENDULUM_H
#define SIMPLEPENDULUM_H

#include <GL/glut.h>
#include <vecmath.h>
#include <vector>

#include "pendulumSystem.h"

using namespace std;

class simplePendulum:public PendulumSystem
{
public:
    simplePendulum();
};
#endif