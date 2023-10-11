#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include <string.h>

using namespace std;

float clampedDepth ( float depthInput, float depthMin , float depthMax);

#include "bitmap_image.hpp"
int main( int argc, char* argv[] )
{
  // Fill in your implementation here.

  // This loop loops over each of the input arguments.
  // argNum is initialized to 1 because the first
  // "argument" provided to the program is actually the
  // name of the executable (in our case, "a4").
  for( int argNum = 1; argNum < argc; ++argNum )
    {
      std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }
	
    
  // First, parse the scene using SceneParser.
  SceneParser sceneParser = SceneParser(argv[2]);
  
  // this one are needed for the first sphere
  Camera* camera = sceneParser.getCamera();
  Group* group = sceneParser.getGroup();

  Vector3f ambientlight = sceneParser.getAmbientLight();
  Vector3f backgroundcolor = sceneParser.getBackgroundColor();

  // Then loop over each pixel in the image, shooting a ray
  // through that pixel and finding its intersection with
  // the scene. Write the color at the intersection to that
  // pixel in your output image.
  int width = atoi(argv[4]);
  int hight = atoi(argv[5]);

  //initialize the image with its size
  Image image( width , hight);

  image.SetAllPixels(backgroundcolor);

  for(int x=0; x< width; x++)
  {
    for(int y=0; y<hight; y++)
    { 
      //normalize point
      float x_normalized = (float(x)-0.5*width)/(0.5*width);
      float y_normalized = (float(y)-0.5*hight)/(0.5*hight); 

      Vector2f point = Vector2f(x_normalized, y_normalized);

      //shout ray 
      Ray ray = camera -> generateRay(point);
      float tmin = camera -> getTMin();
      Hit hit;
      
      Vector3f pixel_color = Vector3f(0.0, 0.0, 0.0);
      if(group -> intersect(ray, hit, tmin))
      {
        //std::cerr << "intersect " << std::endl;
        
        for (int lightidx = 0; lightidx < sceneParser.getNumLights(); lightidx++)
        {
          Light* light = sceneParser.getLight(lightidx);
          Vector3f dirToLight;
          Vector3f lightColor;
          float distanceToLight;
          light -> getIllumination(ray.pointAtParameter(hit.getT()), dirToLight, lightColor, distanceToLight);
          Vector3f shade = hit.getMaterial()->Shade( ray, hit,  dirToLight, lightColor);
          pixel_color += shade;
        }
        
        pixel_color += hit.getMaterial()->getDiffuseColor()*ambientlight;
        image.SetPixel(x, y, pixel_color);
      }      
    } 
  } 
  image.SaveImage(argv[7]);
  return 0;
}

