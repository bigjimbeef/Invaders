#include "MathsHelper.h"

float MathsHelper::Ceil( float& input, float max )
{
	if ( input > max )
	{
		input = max;
	}

	return input;
}

float MathsHelper::Floor( float& input, float floor )
{
	if ( input < floor )
	{
		input = floor;
	}

	return input;
}

float MathsHelper::Clamp( float& input, float min, float max )
{
	if ( input > max )
	{
		input = max;
	}
	else if ( input < min )
	{
		input = min;
	}

	return input;
}

float MathsHelper::Lerp( float& input, float target, float percent )
{
	input += ( percent * ( target - input ) );
	return input;
}

float MathsHelper::Abs( float& input )
{
	return abs( input );
}

float MathsHelper::Round( float& input )
{
	return floor( input + 0.5f );
}