#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#pragma once

#include "PreDec.h"

namespace MocapGE
{
	class Viewport
	{
	public:
		Viewport(void);
		~Viewport(void);

	protected:
		int left;
		int top;
		int width;
		int height;

		Camera* camera_;
	};
}

#endif
