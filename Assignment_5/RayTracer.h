#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <string.h>
#include <cassert>
#include <vecmath.h>

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"
#include "Noise.h"
#include "Light.h"
#include "SceneParser.h"

using namespace std;


class RayTracer
{
public:

    RayTracer(SceneParser* s, Group* g, int max_bounces)
    {
        sceneParser = s;
        group = g; 
        maxBounces = max_bounces;
    }
  
    Vector3f TraceRay( const Ray& ray, Hit& hit, const float tmin, int bounce, bool air = true)
    {

        Vector3f pixel_color = Vector3f(0.0, 0.0, 0.0);
        if(group -> intersect(ray, hit, tmin))
        {
            // the base color
            pixel_color += hit.getMaterial()->getDiffuseColor()*sceneParser->getAmbientLight();

            // light
            for (int lightidx = 0; lightidx < sceneParser->getNumLights(); lightidx++)
            {
                Light* light = sceneParser->getLight(lightidx);
                Vector3f dirToLight;
                Vector3f lightColor;
                float distanceToLight;
                light -> getIllumination(ray.pointAtParameter(hit.getT()), dirToLight, lightColor, distanceToLight);
                
                // Shadow
                if(TraceRayForShadow(ray.pointAtParameter(hit.getT()), dirToLight, distanceToLight, tmin))
                {
                    Vector3f shade = hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor);
                    pixel_color += shade;
                }
            }

            if(bounce == maxBounces)
            {
            return pixel_color;
            }

            // Reflection Depands on when you want to have reflection
            if(true)
            //if(hit.getMaterial()->getShininess() > 0|| hit.getMaterial()->checkNoise())
            {
                Vector3f reflectionDirection = mirrorReflectionAngel( hit.getNormal(), ray.getDirection());
                Ray new_ray = Ray(ray.pointAtParameter(hit.getT()), reflectionDirection);
                Hit new_hit; 
                
                bounce += 1;
                Vector3f reflectionColor = TraceRay(new_ray, new_hit, tmin, bounce, air) * hit.getMaterial()->getSpecularColor();


                // refraction
                if(hit.getMaterial()->getRefractionIndex() > 0)
                {   
                    // to check if the ray go's in or out an object
                    float index_n;
                    float index_nt;
                    Vector3f normal;
                    if(air==false)
                    {
                        index_n = hit.getMaterial()->getRefractionIndex();
                        index_nt = 1.0f;
                        normal = -1* hit.getNormal();
                        air = true;
                    }   
                    else
                    {
                        index_n = 1.0f;
                        index_nt = hit.getMaterial()->getRefractionIndex();
                        normal = hit.getNormal();
                        air = false;
                    }
                    
                    Vector3f newDirection= Vector3f(0.0f, 0.0f, 0.0f);
                    if(transmittedDirection(normal, ray.getDirection(), index_n, index_nt, newDirection))
                    {
                        Ray new_ray = Ray(ray.pointAtParameter(hit.getT()), newDirection);
                        Hit new_hit;        
                        Vector3f colorRefractedRay = TraceRay(new_ray, new_hit, tmin, bounce, air);

                        float R = SchlicksApproximation(normal, ray.getDirection(), newDirection, index_n, index_nt);                     
                        pixel_color  += R*reflectionColor + (1-R)*colorRefractedRay;
                    }
                    else {pixel_color += reflectionColor;}
                }
                else {pixel_color += reflectionColor;}
            }
        }
        else
        { 
        pixel_color = sceneParser->getBackgroundColor(ray.getDirection());
        }

        return pixel_color;
    }





    bool TraceRayForShadow( const Vector3f& origin, const Vector3f& dirToLight, const float distanceToLight, const float tmin) const
    {
        Hit hit_tmp = Hit(distanceToLight, NULL, NULL);
        Ray shadow_ray = Ray(origin, dirToLight);

        if(group -> intersect(shadow_ray, hit_tmp, tmin))
        {
            return false;
        }
        return true;
    }


    Vector3f mirrorReflectionAngel(const Vector3f& normal, const Vector3f incomming) const
    {
        Vector3f reflectionDirection = (incomming - 2.0 * Vector3f::dot(incomming, normal)* normal).normalized();
        return reflectionDirection;
    }


    bool transmittedDirection( const Vector3f& normal, const Vector3f& incoming, float index_n, float index_nt, Vector3f& newDirection) const
    {
        float underRoot = 1-(pow(index_n,2)*(1-pow(Vector3f::dot(incoming, normal),2)))/pow(index_nt,2);
        if(underRoot<0)
        {
            return false;
        }
        newDirection = (index_n*(incoming - normal*Vector3f::dot(incoming, normal)))/index_nt - normal*sqrt(underRoot);
        return true;
    }


    float SchlicksApproximation(const Vector3f& normal, const Vector3f& incoming, const Vector3f& newDirection, float index_n, float index_nt) const
    {
        float R_0 = pow((index_nt-index_n)/(index_nt+index_n), 2);

        float c;
        if(index_n>index_nt)
        {
            c = abs(Vector3f::dot(newDirection, normal));
        }
        else{ c= abs(Vector3f::dot(incoming, normal)); }

        float R = R_0 +(1-R_0)*pow(1-c,5);
        return R;
    }


private:
    SceneParser* sceneParser;
    Group* group;
    int maxBounces; 
};

#endif
//std::cout << "print:" << ... << std::endl;