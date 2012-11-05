#include "Light.h"

namespace MocapGE
{
	Light::~Light(void)
	{
	}


	PointLight::PointLight( void )
		:Light(LT_POINT)
	{
		pos_ = float3(0, 0, 0);
	}

	PointLight::~PointLight( void )
	{

	}

}
