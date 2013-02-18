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
			light_color_ = float4(1, 1, 1, 1);
		}

		void SetType(LightType type);
		LightType GetType(){return light_type_;};
		void SetColor(float4 color);
		float4 GetColor(){return light_color_;};

		void AddToScene();

	protected:
		float4 light_color_;
		LightType light_type_;
	};

	class PointLight : public Light
	{
	public:
		PointLight(void);
		~PointLight(void);

		float3 GetPos(){return pos_;};
		void SetPos(float3 pos);

	private:
		float3 pos_;
	};

	class SpotLight : public Light
	{
	public:
		SpotLight(void);
		~SpotLight(void);

		float3 GetPos(){return pos_;};
		void SetPos(float3 pos);

		float3 GetDir(){return dir_;};
		void SetDir(float3 dir);

	private:
		float3 pos_;
		float3 dir_;
		
		float theta_;
	};
}

#endif
