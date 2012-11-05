#ifndef LIGHT_H_
#define LIGHT_H_

#pragma once
#include "PreDec.h"
#include "TypeAndFormat.h"

namespace MocapGE
{
	class Light
	{
	public:
		~Light(void);

		explicit Light(LightType light_type)
			:light_type_(light_type)
		{
			light_color_ = float4(0, 0, 0, 0);
		}

	protected:
		float4 light_color_;
		LightType light_type_;
	};

	class PointLight : public Light
	{
	public:
		PointLight(void);
		~PointLight(void);

	private:
		float3 pos_;
	};
}

#endif
