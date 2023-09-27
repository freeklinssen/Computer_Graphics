
#include "chain.h"
#include <iostream>
#include "simplePendulumSystem.h"

using namespace std;


simplePendulum::simplePendulum():PendulumSystem(2)
{
    draw_faces = false;
    draw_springs = true;

    particles.push_back(Vector4f(-1.0f, 0.0f, 0.0f, 1.0f));
    particles.push_back(Vector4f(-2.0f, 1.0f, 0.5f, 0.0f));

    // spring defenition: (particle_1, particle_2, string_cons, rest_length)
    springs.push_back(Vector4f(0.0f, 1.0f, 2.0f, 1.0f));

    initPendulum();
}