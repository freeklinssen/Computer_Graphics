#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
using namespace std;
///TODO: Implement Plane representing an infinite plane
///choose your representation , add more fields and fill in the functions
class Plane: public Object3D
{
public:
	Plane(){}
	Plane( const Vector3f& n , float d , Material* material):Object3D(material)
	{
		normal = n;
		D = d;
	}

	~Plane(){}

	virtual bool intersect( const Ray& r , Hit& h , float tmin)
	{
		float t=h.getT();
		Vector3f origin = r.getOrigin();
        Vector3f direction = r.getDirection();

		 
		float denominator = Vector3f::dot(normal, direction);


		if(denominator > 0.02)
		{
			// calculation for intersection with a plain
			float t_new = (Vector3f::dot(normal, origin)+D)/denominator;

			if(t_new<t && t_new>tmin)
			{
				h.set(t_new, material, normal);
			}
		}	
	}

protected:
	Vector3f normal; 
	float D;

};
#endif //PLANE_H
		

