#ifndef CAMERA_H_
#define CAMERA_H_

#pragma once

#include "PreDec.h"
#include "Frustum.h"
#include "Vector.h"
#include "Matrix.h"
#include "Math.h"

namespace MocapGE
{
	class Camera
	{
	public:
		Camera(void);
		Camera(float aspect);
		~Camera(void);

		void SetView(float3 eye, float3 at, float3 up);
		float4x4 GetViewMatirx(){return view_matrix_;};
		void SetProjection(float fovy, float aspect, float zn, float zf);
		float4x4 GetProjMatrix(){return proj_matrix_;};

		float3 GetPos(){return eye_;};
		

	private:
		Frustum	frustum_;

		float fovy_;
		float aspect_;
		float zn_;
		float zf_;
		float4x4 proj_matrix_;

		float3 eye_;
		float3 at_;
		float3 up_;
		float4x4 view_matrix_;

	};

}

#endif

