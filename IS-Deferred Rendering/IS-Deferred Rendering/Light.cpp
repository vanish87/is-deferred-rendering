#include "Light.h"
#include "Context.h"

namespace MocapGE
{
	Light::~Light(void)
	{
	}

	void Light::SetType( LightType type )
	{
		this->light_type_ = type;
	}

	void Light::SetColor( float4 color )
	{
		this->light_color_ = color;
	}

	void Light::AddToScene()
	{
		Context::Instance().GetSceneManager().AddLight(this);
	}


	PointLight::PointLight( void )
		:Light(LT_POINT)
	{
		pos_ = float3(0, 0, 0);
	}

	PointLight::~PointLight( void )
	{

	}

	void PointLight::SetPos( float3 pos )
	{
		pos_ = pos;
	}

}
