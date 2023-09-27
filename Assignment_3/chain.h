#ifndef CHAIN_H
#define CHAIN_H

#include <GL/glut.h>
#include <vecmath.h>
#include <vector>

#include "pendulumSystem.h"

using namespace std;



class chainSystem:public PendulumSystem
{
public:
    chainSystem();
};

#endif