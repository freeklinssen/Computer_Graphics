#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "RayTracer.h"
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
  SceneParser* sceneParser = new SceneParser(argv[2]);
  
  Camera* camera = sceneParser->getCamera();
  Group* group = sceneParser->getGroup();

  Vector3f ambientlight = sceneParser->getAmbientLight();

  RayTracer ray_tracer = RayTracer(sceneParser, group, 4);
  

  // Then loop over each pixel in the image, shooting a ray
  // through that pixel and finding its intersection with
  // the scene. Write the color at the intersection to that
  // pixel in your output image.
  int width = atoi(argv[4]);
  int hight = atoi(argv[5]);

  //initialize the image with its size
  Image image( width , hight);

  for(int x=0; x< width; x++)
  {
    for(int y=0; y<hight; y++)
    { 
      //normalize point
      float x_normalized = (float(x)-0.5*width)/(0.5*width);
      float y_normalized = (float(y)-0.5*hight)/(0.5*hight); 

      //shoot ray 
      Vector2f point = Vector2f(x_normalized, y_normalized);
      Ray ray = camera -> generateRay(point);
      float tmin = camera -> getTMin();
      Hit hit;
      int bounces = 0;

      // the new loop:
      Vector3f pixel_color = ray_tracer.TraceRay(ray, hit, tmin, bounces);
      image.SetPixel(x, y, pixel_color);
    } 
  } 
  image.SaveImage(argv[7]);
  return 0;
}

