#include "clothSystem.h"
#include <iostream>
#include <cmath>

using namespace std;


//TODO: Initialize here
clothSystem::clothSystem(int cloth_width,int cloth_hight):PendulumSystem(cloth_width * cloth_hight)
{
	draw_springs = false;
	draw_faces = true;

	// initialize particles
	int start_hight = 4.0f;
	float rest_lenght = 0.10f;
	float spring_cons = 300.0f;
	float space_between_particles = 0.10;
	float x;
	float z;

	for(int i=0; i<cloth_width; i++)
	{
		for(int j=0; j<cloth_hight; j++)
		{
			//particles
			//fix one point:


			x = i * space_between_particles;
			z = j * space_between_particles;
			//&& j==0) || (i==cloth_width-1 && j==cloth_hight-1)
			
			if((i==0 && j==0) || (i==cloth_width-1 && j==cloth_hight-1) || (i== cloth_width-1   && j==0) )
			{
				particles.push_back(Vector4f(x, start_hight, z, 1.0f));
			}
			else
			{
				particles.push_back(Vector4f(x, start_hight, z, 0.0f));
			}
			

			// initialize springs
			// vertical & horozontal
			
			

			if(i != (cloth_width-1))
			{
				springs.push_back(Vector4f(int(i*cloth_width+j), int((i+1)*cloth_width+j), spring_cons, rest_lenght));
			}
			if(j != (cloth_hight-1))
			{
				springs.push_back(Vector4f(int(i*cloth_width+j), int(i*cloth_width+(j+1)), spring_cons, rest_lenght));
			}

			

			//flex_springs
			if(i < cloth_width-2)
			{
				springs.push_back(Vector4f(int(i*cloth_width+j), int((i+2)*cloth_width+j), spring_cons, rest_lenght*2));
			}
			if(j < cloth_hight-2)
			{
				springs.push_back(Vector4f(int(i*cloth_width+j), int(i*cloth_width+(j+2)), spring_cons, rest_lenght*2));
			}

		

			// shear springs
			if(i < cloth_width-1 && j < cloth_hight-1)
			{
				springs.push_back(Vector4f(int(i*cloth_width+j), int((i+1)*cloth_width+j+1), spring_cons, rest_lenght*sqrt(2)));
			}
			if(j < cloth_hight-1 && j < 0)
			{
				springs.push_back(Vector4f(int(i*cloth_width+j), int((i+1)*cloth_width+j-1), spring_cons, rest_lenght*sqrt(2)));
			}

			// faces

			if(i < cloth_width-1 && j < cloth_hight-1)
			{
			// pointing backwards
			faces.push_back(Tuple3u(int(i*cloth_width+j), int((i+1)*cloth_width+j), int(i*cloth_width+j+1)));
			faces.push_back(Tuple3u(int(i*cloth_width+j+1), int((i+1)*cloth_width+j), int((i+1)*cloth_width+j+1)));	

			// pointing forward
			faces.push_back(Tuple3u(int(i*cloth_width+j), int(i*cloth_width+(j+1)), int((i+1)*cloth_width+j)));
			faces.push_back(Tuple3u(int(i*cloth_width+j+1), int((i+1)*cloth_width+(j+1)), int((i+1)*cloth_width+j)));	


			}
		}
	
	}

	initPendulum();
}

