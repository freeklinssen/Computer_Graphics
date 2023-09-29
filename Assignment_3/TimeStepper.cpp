#include "TimeStepper.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;


///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    vector<Vector3f>  new_vVecState; 
    vector<Vector3f> grads;
    vector<Vector3f> state;

    state = particleSystem -> getState();
    grads = particleSystem -> evalF(state);

    for(unsigned i=0; i<state.size(); i++)
    {   
        state[i] = state[i] + stepSize*grads[i];
    }

    particleSystem -> setState(state);
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    vector<Vector3f> grads_0;
    vector<Vector3f> grads_1;
    vector<Vector3f> state;
    vector<Vector3f> euler;

    state = particleSystem -> getState();
    grads_0 = particleSystem -> evalF(state);

    for(unsigned i=0; i<state.size(); i++)
    { 
        euler.push_back(state[i] + stepSize*grads_0[i]);
    }
    grads_1 = particleSystem -> evalF(euler);


    for(unsigned i=0; i<state.size(); i++)
    {
        state[i] = state[i] + stepSize*0.5*(grads_0[i] + grads_1[i]);
    }
    particleSystem -> setState(state);
}
