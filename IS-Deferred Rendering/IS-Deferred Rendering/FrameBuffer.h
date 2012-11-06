#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#pragma once

#include "PreDec.h"
#include "RenderView.h"
#include "Viewport.h"

namespace MocapGE
{
	class FrameBuffer
	{
	public:
		FrameBuffer(void);
		~FrameBuffer(void);

		Camera* GetFrameCamera();
		virtual void OnBind() = 0;


	protected:
		uint32_t	left_;
		uint32_t	top_;
		uint32_t	width_;
		uint32_t	height_;

		Viewport* viewport_;

		RenderView* render_view_;
	};
}

#endif

