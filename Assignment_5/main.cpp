#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <random>

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
  int width = atoi(argv[4])*3;
  int hight = atoi(argv[5])*3;
  int original_width = atoi(argv[4]);
  int original_hight = atoi(argv[5]);

  //initialize the image with its size
  Image image(width, hight);
  Vector3f pixel_color = Vector3f(0.0f, 0.0f, 0.0f);


  for(int x=0; x< width; x++)
  {
    for(int y=0; y<hight; y++)
    { 
      // Generate a random number between 0 and 1
      float x_r = x + (rand()/RAND_MAX - 0.5);
      float y_r = y + (rand()/RAND_MAX - 0.5);
    
      //normalize point
      float x_normalized = (x_r-0.5*width)/(0.5*width);
      float y_normalized = (y_r-0.5*hight)/(0.5*hight); 

      //shoot ray 
      Vector2f point = Vector2f(x_normalized, y_normalized);
      Ray ray = camera -> generateRay(point);
      float tmin = camera -> getTMin();
      Hit hit;
      int bounces = 0;

      // the ray tracing loop:
      pixel_color = ray_tracer.TraceRay(ray, hit, tmin, bounces);
      image.SetPixel(x, y, pixel_color);
    } 
  } 
  


  // Gaussian Blur
  Image horizontalBlur(width, hight);
  for(int x=0; x< width; x++)
  {
    for(int y=0; y<hight; y++)
    {
      Vector3f xmin2 = (x-2 < 0) ? image.GetPixel(x,y) :  image.GetPixel(x-2,y);
      Vector3f xmin1 = (x-1 < 0) ? image.GetPixel(x,y) :  image.GetPixel(x-1,y);
      Vector3f xplus1 = (x+1 > width-1) ? image.GetPixel(x,y) :  image.GetPixel(x+1,y);
      Vector3f xplus2 = (x+2 > width-1) ? image.GetPixel(x,y) :  image.GetPixel(x+2,y);

      pixel_color = xmin2*0.1201 + xmin1*0.2339 + image.GetPixel(x,y)*0.2931 +  xplus1*0.2339 + xplus2*0.1201; 
      horizontalBlur.SetPixel(x, y, pixel_color);
    }
  }

  Image blurredImage(width, hight);
  for(int x=0; x< width; x++)
  {
    for(int y=0; y<hight; y++)
    {
      Vector3f ymin2 = (y-2 < 0) ? horizontalBlur.GetPixel(x,y) :  horizontalBlur.GetPixel(x,y-2);
      Vector3f ymin1 = (y-1 < 0) ? horizontalBlur.GetPixel(x,y) :  horizontalBlur.GetPixel(x,y-1);
      Vector3f yplus1 = (y+1 > hight-1) ? horizontalBlur.GetPixel(x,y) :  horizontalBlur.GetPixel(x,y+1);
      Vector3f yplus2 = (y+2 > hight-1) ? horizontalBlur.GetPixel(x,y) :  horizontalBlur.GetPixel(x,y+2);

      pixel_color = ymin2*0.1201 + ymin1*0.2339 + horizontalBlur.GetPixel(x,y)*0.2931 +  yplus1*0.2339 + yplus2*0.1201; 
      blurredImage.SetPixel(x, y, pixel_color);

    }
  }

  
  // Down-sample the image.
  Image finalImage(original_width, original_hight);

  for(int x=0; x< original_width; x++)
  {
    for(int y=0; y<original_hight; y++)
    {
      pixel_color = Vector3f(0.0f, 0.0f, 0.0f);
      for(int xx=-1; xx<=1; xx++)
      {
        for(int yy=-1; yy<=1; yy++)
        {
          pixel_color += ((x*3+1+xx) < 0) ? horizontalBlur.GetPixel(3*x+1, 3*y+1): ((3*x+1+xx) > width) ? horizontalBlur.GetPixel(3*x+1, 3*y+1): ((y*3+1+yy) < 0) ? horizontalBlur.GetPixel(3*x+1, 3*y+1): ((3*y+1+yy) > hight) ? horizontalBlur.GetPixel(3*x+1, 3*y+1):  horizontalBlur.GetPixel(3*x+1+xx, 3*y+1+yy);
        }
      }
     finalImage.SetPixel(x, y, pixel_color/9);
    }
  }

  finalImage.SaveImage(argv[7]);
  return 0;
}

