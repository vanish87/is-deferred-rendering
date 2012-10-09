#ifndef CAMERA_H_
#define CAMERA_H_

#pragma once

#include "PreDec.h"
#include "Frustum.h"

namespace MocapGE
{
	class Camera
	{
	public:
		Camera(void);
		~Camera(void);

	private:
		Frustum	frustum_;

	};

}

#endif

