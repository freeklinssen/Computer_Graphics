#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"
///TODO:
///Implement Shade function that uses ambient, diffuse, specular and texture
class Material
{
public:
	
 Material( const Vector3f& d_color ,const Vector3f& s_color=Vector3f::ZERO, float s=0):
  diffuseColor( d_color),specularColor(s_color), shininess(s)
  {
        	
  }

  virtual ~Material(){}

  virtual Vector3f getDiffuseColor() const 
  { 
    return  diffuseColor;
  }
    

  Vector3f Shade( const Ray& ray, const Hit& hit, const Vector3f& dirToLight, const Vector3f& lightColor ) 
    {
    Vector3f normal = hit.getNormal();
    Vector3f direction = ray.getDirection();

    // Specular shading
    Vector3f R = (direction - 2.0 * Vector3f::dot(direction, normal)* normal).normalized();
    float shading_intensity = pow(Vector3f::dot(R, dirToLight), shininess);
    if (Vector3f::dot(R,  dirToLight) < 0){shading_intensity = 0;}

    Vector3f specular_component = shading_intensity * lightColor * specularColor;

    //Diffuse Shading
    if(t.valid()){ diffuseColor = t(hit.texCoord[0], hit.texCoord[1]);}
	  float LdotN = Vector3f::dot(normal, dirToLight);
	  if (LdotN <= 0){LdotN = 0;}

    Vector3f diffuse_component = LdotN*lightColor*diffuseColor;


    return Vector3f(diffuse_component + specular_component); 
  }

  void loadTexture(const char * filename)
  {
    t.load(filename);
  }

 protected:
  Vector3f diffuseColor;
  Vector3f specularColor;
  float shininess;
  Texture t;
};



#endif // MATERIAL_H
