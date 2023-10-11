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
	Plane( const Vector3f& n , float d , Material* m):Object3D(m)
	{
		normal = n;
		D = d;
		material = m;
	}

	~Plane(){}

	virtual bool intersect( const Ray& r , Hit& h , float tmin)
	{
		float t=h.getT();
		Vector3f origin = r.getOrigin();
        Vector3f direction = r.getDirection();


		float denominator = Vector3f::dot(normal, direction);
		if(denominator != 0)
		{
			// calculation for intersection with a plain
			float t_new = -1*(Vector3f::dot(normal, origin)-D)/denominator;

			if( t_new>tmin && t_new<t)
			{
				h.set(t_new, material, normal);
				return true;
			}
			else{return false;}
		}	
	}

protected:
 	Material* material;
	Vector3f normal; 
	float D;

};
#endif //PLANE_H
		

