#include "MyMath.h"

float	MyMath::pow(float x, float y) {
	double m = 1;
	for (int i = 0; i < y; i++)
	{
		m = m * x;
	}
	return	m;
}