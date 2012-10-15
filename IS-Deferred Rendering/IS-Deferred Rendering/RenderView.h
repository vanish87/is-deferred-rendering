#ifndef RENDERVIEW_H_
#define RENDERVIEW_H_

#pragma once

#include "TypeAndFormat.h"
namespace MocapGE
{
	class RenderView
	{
	public:
		RenderView(void);
		~RenderView(void);

	protected:
		Format format_;
		size_t height_;
		size_t width_;

		size_t dimension_; 
	};

}

#endif

