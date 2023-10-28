
#include "pendulumSystem.h"

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
}

void PendulumSystem::initPendulum()
{
	for (int i = 0; i < m_numParticles; i++){
		// for this system, we care about the position and the velocity
		m_vVecState.push_back(particles[i].xyz());
		// velocity
		m_vVecState.push_back(Vector3f(0.0f, 0.0f, 0.0f));
	}
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;

	// Gravity & Drag
	float gravity = -2.0f;
	float drag = 0.3f;

	for(unsigned i=0; i<particles.size(); i++)
	{
		f.push_back(state[2*i+1]);
		Vector3f gravity_force = Vector3f(0.0f, gravity, 0.0f);
		Vector3f drag_force = -1 * drag * state[2*i+1];
		f.push_back(Vector3f(gravity_force + drag_force));
	}

	// Spring
	for(unsigned i=0; i<springs.size(); i++)
	{
		float string_cons = springs[i][2];
		float rest_length = springs[i][3];

		Vector3f particle_1 = state[2*springs[i][0]].xyz();
		Vector3f particle_2 = state[2*springs[i][1]].xyz(); 
		Vector3f d = particle_1 - particle_2;

		Vector3f force_particle_1 = -1 * string_cons * (d.abs() - rest_length) *(d/d.abs());
		Vector3f force_particle_2 = -1 * force_particle_1;

		f[2*springs[i][0]+1] +=  force_particle_1;
		f[2*springs[i][1]+1] +=  force_particle_2;
	}

	// make some particles fixed
	for(unsigned i=0; i<particles.size(); i++)
	{
		if(particles[i][3]==1.0f)
		{
			f[2*i+1] = Vector3f(0.0f, 0.0f, 0.0f);
		}
	}
	return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	if(draw_faces == true)
	{
		glBegin(GL_TRIANGLES);
		for(int i = 0; i < faces.size(); i++)
		{
			//Edge1 = getState()[2*faces[i][1]] - getState()[2*faces[i][0]];
			//Edge2 = getState()[2*faces[i][2]] - getState()[2*faces[i][0]];

			//normal = Vector3f::cross(Edge1, Edge2).normalized();
			//normal *= -1;

			Vector3f normal_1 = getNormal(faces[i][0]);
			Vector3f normal_2 = getNormal(faces[i][1]);
			Vector3f normal_3 = getNormal(faces[i][2]);

			// frontside
			glNormal3d(normal_1[0], normal_1[1], normal_1[2]);
			glVertex3d(getState()[2*faces[i][0]][0], getState()[2*faces[i][0]][1], getState()[2*faces[i][0]][2]);

			glNormal3d(normal_2[0], normal_2[1], normal_2[2]);
			glVertex3d(getState()[2*faces[i][1]][0], getState()[2*faces[i][1]][1], getState()[2*faces[i][1]][2]);

			glNormal3d(normal_3[0], normal_3[1], normal_3[2]);
			glVertex3d(getState()[2*faces[i][2]][0], getState()[2*faces[i][2]][1], getState()[2*faces[i][2]][2]);

			// backside
			glNormal3d(-normal_1[0], -normal_1[1], -normal_1[2]);
			glVertex3d(getState()[2*faces[i][0]][0], getState()[2*faces[i][0]][1], getState()[2*faces[i][0]][2]);

			glNormal3d(-normal_3[0], -normal_3[1], -normal_3[2]);
			glVertex3d(getState()[2*faces[i][2]][0], getState()[2*faces[i][2]][1], getState()[2*faces[i][2]][2]);

			glNormal3d(-normal_2[0], -normal_2[1], -normal_2[2]);
			glVertex3d(getState()[2*faces[i][1]][0], getState()[2*faces[i][1]][1], getState()[2*faces[i][1]][2]);

		}
		glEnd;
		
	}

	else
	{
		for (int i = 0; i < m_numParticles; i++) {
			Vector4f pos ;//  position of particle i. YOUR CODE HERE
			pos = Vector4f(getState()[i*2]);
			glPushMatrix();
			glTranslatef(pos[0], pos[1], pos[2]);
			glutSolidSphere(0.075f,10.0f,10.0f);
			glPopMatrix();
		}
		
		if(draw_springs == true)
		{ 
			/// TODO 
			Vector4f particle_1;
			Vector4f particle_2;
    		for (int i = 0; i < springs.size(); i++) {
				// spring defenition: (particle_1, particle_2, string_cons, rest_length)
				particle_1 = Vector4f(getState()[2*springs[i][0]]);
				particle_2 = Vector4f(getState()[2*springs[i][1]]);
				glLineWidth(2.0f);
				glBegin(GL_LINES);
				glVertex3f(particle_1[0], particle_1[1], particle_1[2]);
				glVertex3f(particle_2[0], particle_2[1], particle_2[2]);
	    		glEnd();

    		}
		}	
	}
}

Vector3f PendulumSystem::getNormal(int verticeIndex)
{
	Vector3f Edge1;
	Vector3f Edge2;
	Vector3f normal;
	Vector3f result = Vector3f(0.0f, 0.0f, 0.0f);
	vector<Tuple3u> faces = adjecentFaces[verticeIndex];

	for (int v=0; v< faces.size(); v++)
	{
		Edge1 = getState()[2*faces[v][1]] - getState()[2*faces[v][0]];
		Edge2 = getState()[2*faces[v][2]] - getState()[2*faces[v][0]];
		normal = Vector3f::cross(Edge1, Edge2).normalized();
		result += normal;
	}
	return result/faces.size();
}
