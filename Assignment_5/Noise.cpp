#include "Noise.h"
#include "PerlinNoise.h"
#include <iostream>
#include <cmath>

Vector3f Noise::getColor(const Vector3f & pos)
{
  //Fill in this function  ONLY.
  float N;
  float M;
  if(type == 1)
  {
  	N = PerlinNoise::octaveNoise(pos, octaves);
  	M = sin(frequency*pos.x() + amplitude*N);
  }
  else
  {
	N = 10*PerlinNoise::octaveNoise(pos, octaves); 
	M = N-(int)N;
  }

  M = (M+1.0f)/2.0f;
  Vector3f result = M*color[0] + (1-M)*color[1];
  

  return result;
}

Noise::Noise(int _octaves,const Vector3f & color1, 
	 const Vector3f &color2,float freq,float amp, float typ):
octaves(_octaves),frequency(freq),amplitude(amp), type(typ){
	color[0] = color1;
	color[1] = color2;
	init = true;

}

Noise::Noise(const Noise & n):
octaves(n.octaves),frequency(n.frequency),
	amplitude(n.amplitude),type(n.type),init(n.init)
{
	color[0] = n.color[0];
	color[1] = n.color[1];

}

bool Noise::valid()
{
	return init;
}

Noise::Noise():
octaves(0),init(false)
{}