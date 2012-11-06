#include "Math.h"

namespace MocapGE
{
	namespace Math
	{
		float Sqrt(float x)
		{
			return std::sqrt(x);
		}

		float Sin(float x)
		{
			return std::sin(x);
		}

		float Cos(float x)
		{
			return std::cos(x);
		}


		float Tan(float x)
		{
			return std::tan(x);
		}

		float Cot(float x)
		{
			return 1.0f / Tan(x);
		}
		float InvSqrt(float number)
		{
			float xhalf = 0.5f*number;

			int i = *(int*)&number; // get bits for floating value
			i = 0x5f3759df - (i>>1); // gives initial guess y0
			number = *(float*)&i; // convert bits back to float
			number = number*(1.5f-xhalf*number*number); // Newton step, repeating increases accuracy
			number = number*(1.5f-xhalf*number*number); // 2nd Newton step, repeating increases accuracy

			return number;
		}
	}
}