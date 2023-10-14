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
  
    Vector3f TraceRay( const Ray& ray, Hit& hit, const float tmin, int bounce) const
    {
        Vector3f pixel_color = Vector3f(0.0, 0.0, 0.0);
        if(group -> intersect(ray, hit, tmin))
        {
            for (int lightidx = 0; lightidx < sceneParser->getNumLights(); lightidx++)
            {
            Light* light = sceneParser->getLight(lightidx);
            Vector3f dirToLight;
            Vector3f lightColor;
            float distanceToLight;
            light -> getIllumination(ray.pointAtParameter(hit.getT()), dirToLight, lightColor, distanceToLight);
            Vector3f shade = hit.getMaterial()->Shade(ray, hit,  dirToLight, lightColor);
            //std::cout << "shade: " << shade[0] << shade[1] << shade[2] << std::endl;
            pixel_color += shade;
            }
            pixel_color += hit.getMaterial()->getDiffuseColor()*sceneParser->getAmbientLight();
        }
        else
        { 
        pixel_color = sceneParser->getBackgroundColor(ray.getDirection());
        }

        return pixel_color;

    }

private:
    SceneParser* sceneParser;
    Group* group;
    int maxBounces; 



};

#endif // RAYTRACER_H

