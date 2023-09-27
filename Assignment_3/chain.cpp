#include "chain.h"
#include <iostream>

using namespace std;



chainSystem::chainSystem():PendulumSystem(4)
{

    draw_faces = false;
    draw_springs = true;

    particles.push_back(Vector4f(-1.0f, 3.0f, 0.0f, 1.0f));
    particles.push_back(Vector4f(-2.0f, 4.0f, 1.5f, 0.0f));
    particles.push_back(Vector4f(-0.0f, 2.0f, 0.0f, 0.0f));
    particles.push_back(Vector4f(-1.0f, 3.0f, -1.5f, 0.0f));

    // spring defenition: (particle_1, particle_2, string_cons, rest_length)
    //float restLength = 1.0f;
    //float springConstant = 2.0f;
    springs.push_back(Vector4f(0.0f, 1.0f, 3.0f, .5f));
    springs.push_back(Vector4f(1.0f, 2.0f, 2.0f, .5f));
    springs.push_back(Vector4f(2.0f, 3.0f, 2.0f, .5f));
    
    initPendulum();

}

