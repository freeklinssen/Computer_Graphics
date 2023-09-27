
#include "simpleSystem.h"

using namespace std;

SimpleSystem::SimpleSystem()
{
	m_vVecState.push_back(Vector3f(1.0f, 0.0f, 0.0f));
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	Vector3f tmp;
	for(int i=0; i<state.size(); i++)
	{
		tmp = Vector3f(-1*state[i][1], state[i][0], 0.0f);
		f.push_back(tmp);
	}
	return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
		Vector3f pos ;//YOUR PARTICLE POSITION
		for(int i=0; i<m_vVecState.size(); ++i)
		{
			pos = getState()[i];
			glPushMatrix();
			glTranslatef(pos[0], pos[1], pos[2]);
			glutSolidSphere(0.075f, 10.0f, 10.0f);
			glPopMatrix();
		}

	    
}
