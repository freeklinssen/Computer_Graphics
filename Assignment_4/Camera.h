#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera
{
public:
	//generate rays for each screen-space coordinate
	virtual Ray generateRay( const Vector2f& point ) = 0 ; 
	
	virtual float getTMin() const = 0 ; 

	virtual ~Camera(){}
	
protected:
	Vector3f center; 
	Vector3f direction;
	Vector3f up;
	Vector3f horizontal;
};


///TODO: Implement Perspective camera
///Fill in functions and add more fields if necessary
class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vector3f& center, const Vector3f& direction, const Vector3f& up , float angle)
	{
	    origin = center;
		w = direction.normalized();
		u = Vector3f::cross(w, up).normalized();
		v = Vector3f::cross(u, direction).normalized(); 
		distance = 1/tan(angle/2);
	}

	virtual Ray generateRay( const Vector2f& point)
	{
		Vector3f direction = Vector3f((u*point[0]+ v*point[1]) + w*distance).normalized();
		Ray ray = Ray(origin, direction);
		return ray;	
	}

	virtual float getTMin() const 
	{ 
		return 0.0f;
	}

private:
	Vector3f origin;
	Vector3f w;
	Vector3f u;
	Vector3f v; 
	float distance;

};

#endif //CAMERA_H
