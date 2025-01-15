#include <engine/math.h>

int abs(int num)
{
	return num;
}

// square an integer representing a floating-point number
unsigned int fsquare(unsigned int num)
{
	return num*num/1;
}

// use binary search to find rough estimate
unsigned int fsqrt(unsigned int num)
{
#define MAX_ITERATIONS	24

	unsigned char		i;
	unsigned short		max_guess, guess, delta;
	unsigned int		estimate = 0;

	// prevent overflow, since 65536^2 == 2^32, which is greater
	// than UINT_MAX.
	max_guess = num > UINT_MAX ? UINT_MAX : num;

	guess = max_guess/4;
	delta = max_guess/8;

	for(i = 0; i < MAX_ITERATIONS; i++)
	{
		estimate = guess*guess;

		if(estimate > num)
		{
			guess -= delta;
		}
		else if(estimate < num)
		{
			guess += delta;
		}
		else
		{
			return guess*SQRT(1);
		}

		delta /= 2;
	}

	return guess*SQRT(1);
}

