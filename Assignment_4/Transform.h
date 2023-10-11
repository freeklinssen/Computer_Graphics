#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "Object3D.h"
///TODO implement this class
///So that the intersect function first transforms the ray
///Add more fields as necessary
class Transform: public Object3D
{
public: 
  Transform(){}
 Transform( const Matrix4f& m, Object3D* obj):o(obj)
 {
    matrix = m;  
 }

  ~Transform(){}

  virtual bool intersect( const Ray& r , Hit& h , float tmin)
  {
    
    Vector4f origin4 = Vector4f(r.getOrigin(), 1.0f);
    Vector4f direction4 = Vector4f(r.getDirection(), 0.0f);
    
    Vector3f transformed_origin = (matrix.inverse() * origin4).xyz();
    Vector3f transformed_direction = (matrix.inverse() * direction4).xyz();

    Ray transformed_ray = Ray(transformed_origin, transformed_direction);
    

    if(o->intersect(transformed_ray, h, tmin)){    
      Vector4f normal4 = Vector4f(h.getNormal(), 0.0f);
      Vector3f normal = (matrix.inverse().transposed() * normal4).normalized().xyz().normalized();
      h.setNormal(normal);
      return true;
    }
    else{return false;}
  }

 protected:
  Object3D* o; //un-transformed object	
  Matrix4f matrix;
};

#endif //TRANSFORM_H
