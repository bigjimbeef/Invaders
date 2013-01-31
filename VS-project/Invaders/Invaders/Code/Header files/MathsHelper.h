#ifndef D_MATHSHELPER_H
#define D_MATHSHELPER_H

//-----------------------------------------------------------------------------
// MathsHelper.h
//-----------------------------------------------------------------------------
// A helper class providing a wrapper for various maths functions
//-----------------------------------------------------------------------------

#include <cmath>

class MathsHelper
{
	public:
		//-------------------------------------------------------------------------
		// Static accessors for common mathematical constants.

		static const float PI()			{ return 3.1415926535897932384626433833f; }
		static const float TWOPI()		{ return 6.2831853071795864769252867666f; }
		static const float PIBYTWO()	{ return 1.5707963267948966192313216916f; }
		static const float PIBYFOUR()	{ return 0.7853981633974483096156608458f; }
		static const float E()			{ return 2.7182818284590452353602874714f; }

		//-------------------------------------------------------------------------
		// Static functions

		// Clamps a float between minimum and maximum values.
		static float Clamp( float& input, float min, float max );

		// Clamps a float to a maximum.
		static float Ceil( float& input, float ceil );

		// Clamps a float to a minimum.
		static float Floor( float& input, float floor );

		// Linearly interpolate a float towards a target value.
		static float Lerp( float& input, float target, float percent );

		// Gets the absolute of an input value.
		static float Abs( float& input );

		// Return the nearest integer to a float.
		static float Round( float& input );

	private:

		// Making it an abstract class. Don't want anyone instantiating this.
		virtual void CLASS_IS_ABSTRACT() = 0;
};

#endif // D_MATHSHELPER_H