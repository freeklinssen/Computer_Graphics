#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
#include <iostream>

using namespace std;
///TODO: implement this class.
///Add more fields as necessary,
///but do not remove hasTex, normals or texCoords
///they are filled in by other components
class Triangle: public Object3D
{
public:
	Triangle();
        ///@param a b c are three vertex positions of the triangle
	Triangle( const Vector3f& A, const Vector3f& B, const Vector3f& C, Material* m):Object3D(m)
	{
		a = A;
		b = B;
		c = C;
		material = m;
        hasTex = false;
	}

	virtual bool intersect( const Ray& r,  Hit& h , float tmin)
	{
		float t=h.getT();
		Vector3f origin = r.getOrigin();
        Vector3f direction = r.getDirection();

		Matrix3f M = Matrix3f(a[0]-b[0], a[0]-c[0], direction[0],
							  a[1]-b[1], a[1]-c[1], direction[1],
							  a[2]-b[2], a[2]-c[2], direction[2]);

		Matrix3f t_matrix = Matrix3f(a[0]-b[0], a[0]-c[0], a[0]-origin[0],
									 a[1]-b[1], a[1]-c[1], a[1]-origin[1],
									 a[2]-b[2], a[2]-c[2], a[2]-origin[2]);

		Matrix3f beta_matrix = Matrix3f(a[0]-origin[0], a[0]-c[0], direction[0],
									    a[1]-origin[1], a[1]-c[1], direction[1],
									    a[2]-origin[2], a[2]-c[2], direction[2]);

		Matrix3f gamma_matrix = Matrix3f(a[0]-b[0], a[0]-origin[0], direction[0],
									     a[1]-b[1], a[1]-origin[1], direction[1],
									     a[2]-b[2], a[2]-origin[2], direction[2]);

		float t_new = t_matrix.determinant() / M.determinant();
		float beta = beta_matrix.determinant() / M.determinant();
		float gamma = gamma_matrix.determinant() / M.determinant();


		if(0<=beta && beta<=1 && 0<=gamma && gamma<=1 && (beta+gamma)<=1 && tmin<t_new && t_new<t)
		{
			Vector3f normal = ((1-beta-gamma)* normals[0] + beta*normals[1] + gamma*normals[2]).normalized();
			h.set(t_new, material, normal);
			h.setTexCoord(((1-beta-gamma)* texCoords[0] + beta*texCoords[1] + gamma*texCoords[2]));
			
			return true;
		}
		else{return false;} 
	}

	bool hasTex;
	Vector3f normals[3];
	Vector2f texCoords[3];

protected:

	Vector3f a;
	Vector3f b;
	Vector3f c;
	Material* material;


};

#endif //TRIANGLE_H
