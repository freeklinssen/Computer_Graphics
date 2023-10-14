#include "Noise.h"
#include "PerlinNoise.h"
#include <iostream>
#include <cmath>

Vector3f Noise::getColor(const Vector3f & pos)
{
  //Fill in this function  ONLY.
  //INTERPOLATE BETWEEN TWO COLORS BY WEIGHTED AVERAGE
  

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