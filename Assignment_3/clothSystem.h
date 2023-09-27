#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vecmath.h>
#include <vector>
#include <GL/glut.h>


#include "pendulumSystem.h"


class clothSystem: public PendulumSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	clothSystem(int width, int hight);
};
#endif
