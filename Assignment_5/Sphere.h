#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

#include <iostream>
using namespace std;
///TODO:
///Implement functions and add more fields as necessary
class Sphere: public Object3D
{
public:
	Sphere(){ 
		//unit ball at the center
	}

	Sphere( Vector3f center, float radius, Material* material):Object3D(material)
	{
		Center = center;
		Radius = radius;
	}	

	~Sphere(){}

	virtual bool intersect(const Ray& r, Hit& h, float tmin){
		
		float t = h.getT();

		Vector3f normal;
		Vector3f origin = (r.getOrigin()-Center);
        Vector3f direction = r.getDirection();

		// ABC formula
		float a = Vector3f::dot(direction, direction);  // should always be one since direction is normalized
		float b = 2 * Vector3f::dot(origin, direction);
		float c = Vector3f::dot(origin, origin) - Radius*Radius;

		float discriminant = sqrt(b*b-4*a*c);


		if(discriminant >= 0)
			//does discriminant == 0 realy happen??
			// don't think we need to implement this, in that case we just treat it as < 0
		{
			float t_1 = (-b+discriminant)/(2*a);
			float t_2 = (-b-discriminant)/(2*a);

			bool Bool = false;
			if(t_1>tmin && t_2>tmin)
			{
				if(t_1<t_2 && t_1<t)
				{
					normal = (r.pointAtParameter(t_1)-Center).normalized();
					h.set(t_1, material, normal);
					Bool = true;
				}
				else if(t_2<t)
				{
					normal = (r.pointAtParameter(t_2)-Center).normalized();
					h.set(t_2, material, normal);
					Bool = true;

				}
			}
			else if(t_1>tmin && t_1<t)
			{
				normal = (r.pointAtParameter(t_1)-Center).normalized();
				h.set(t_1, material, normal);
					Bool = true;

			}	
			else if(t_2>tmin && t_2<t)
			{
				normal = (r.pointAtParameter(t_2)-Center).normalized();
				h.set(t_2, material, normal);
					Bool = true;

			} 
		 	return Bool;
		}
		return false;
	}

protected:
	Vector3f Center;
	float Radius;
};


#endif
