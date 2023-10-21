
#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"
#include "Noise.h"


class Material
{
public:

 Material( const Vector3f& d_color ,const Vector3f& s_color=Vector3f::ZERO, float s=0, float r=0):
  diffuseColor( d_color), specularColor(s_color), shininess(s), refractionIndex(r)
  {
    noiseExists = false;
  }

  virtual ~Material(){}

  virtual Vector3f getDiffuseColor() const
  {
    return  diffuseColor;
  }

  Vector3f getSpecularColor()
  {
    return specularColor;
  }

  float getRefractionIndex()
  {
    return refractionIndex;
  }

  float getShininess()
  {
    return shininess;
  }

  Noise getNoise()
  {
    return noise;
  }
  
  bool checkNoise()
  {
    if(noiseExists)
    {
      return true;
    }
    return false;
  }
  

 
  Vector3f Shade(const Ray& ray, const Hit& hit, const Vector3f& dirToLight, const Vector3f& lightColor)
  {
    Vector3f normal = hit.getNormal();
    Vector3f color;
    
    //Diffuse light
    if(t.valid()){ color = t(hit.texCoord[0], hit.texCoord[1]);}
    else{color = diffuseColor;}
    if(noiseExists)
    {
      color = noise.getColor(ray.pointAtParameter(hit.getT()));
    }

    float LdotN = Vector3f::dot(normal, dirToLight);
    if (LdotN <= 0){LdotN = 0;}
    return LdotN * lightColor * color; 
  }


  void loadTexture(const char * filename)
  {
    t.load(filename);
  }

  void setNoise(const Noise& n)
  {
    noiseExists = true; 
    noise = n;
  }


protected:
  Vector3f diffuseColor;
  Vector3f specularColor;
  float shininess;
  float refractionIndex;
  Texture t;
  bool noiseExists;
  Noise noise;
};

#endif // MATERIAL_H