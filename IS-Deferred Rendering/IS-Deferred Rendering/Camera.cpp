#include "Camera.h"

namespace MocapGE
{
	Camera::Camera(void)
	{
		this->SetView(float3(0,0,-1), float3(0,0,0), float3(0,1,0));
		this->SetProjection(Math::PI/4, 1 , 1, 1000);
		//TODO : Init frustum here
		frustum_;
	}

	Camera::Camera(float aspect)
	{
		this->SetView(float3(0,0,-1), float3(0,0,0), float3(0,1,0));
		this->SetProjection(Math::PI/4, aspect , 1, 1000);
		frustum_;
	}

	Camera::~Camera(void)
	{
	}

	void Camera::SetView( float3 eye, float3 at, float3 up )
	{
		eye_ = eye;
		at_ = at;
		up_ = up;

		view_matrix_ = Math::LookAtLH(eye_, at_, up_);
	}

	void Camera::SetProjection( float fovy, float aspect, float zn, float zf )
	{
		fovy_ = fovy;
		aspect_ = aspect;
		zn_ = zn;
		zf_ = zf;

		proj_matrix_ = Math::PerspectiveFovLH(fovy_, aspect_, zn_, zf_);
	}

}
