#include "Easing.h"

float	easeOutBack(float num) {
	const	float c1 = 1.70158;
	const	float c2 = c1 + 1;

	return	1 + c2 * MyMath::pow(num - 1, 3) + c1 * MyMath::pow(num - 1, 2);
}

float	easeInSine(float num) {
	return	1 - cos((num * MathUtility::PI) / 2);
}