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

	SpotLight::SpotLight( void )
		:Light(LT_SPOT)
	{
		pos_ = float3(0, 0, 0);
		dir_ = float3(0, 0, -1);

		inner_angle_ = Math::PI / 6;
		outer_angle_ = Math::PI / 4;
	}

	SpotLight::~SpotLight( void )
	{

	}

	void SpotLight::SetPos( float3 pos )
	{
		pos_ = pos;
	}

	void SpotLight::SetDir( float3 dir )
	{
		dir_ = dir;
	}

	void SpotLight::SetOuterAngle( float outer_angle )
	{
		outer_angle_ = outer_angle;
	}

	void SpotLight::SetInnerAngle( float inner_angle )
	{
		inner_angle_ = inner_angle;
	}



}
