#include "Camera.h"

namespace MocapGE
{
	Camera::Camera(void)
	{
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
