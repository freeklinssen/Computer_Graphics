#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include <vecmath.h>
#include <vector>
#include <GL/glut.h>

#include "particleSystem.h"
#include "tuple.h"
typedef tuple<unsigned,3> Tuple3u;


class PendulumSystem: public ParticleSystem
{
public:
	PendulumSystem(int numParticles);
	
	void initPendulum();
	
	vector<Vector3f> evalF(vector<Vector3f> state);
	
	void draw();

	Vector3f getNormal(int verticeIndex);

protected:

	vector<Vector4f> particles;
	vector<vector<Tuple3u> > adjecentFaces;
	vector<Vector4f> springs;
	vector<Tuple3u> faces;

	// info for drawing
	bool draw_springs;
	bool draw_faces;
};

#endif
