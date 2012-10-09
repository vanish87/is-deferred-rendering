#ifndef RENDERENGINE_H_
#define RENDERENGINE_H_

#pragma once

#include <string>

#include "PreDec.h"
#include "Configure.h"

namespace MocapGE
{
	class MOCAPGE_API RenderEngine
	{
	public:
		RenderEngine(void);
		~RenderEngine(void);
		virtual void InitRenderWindow(std::string const & name, RenderSetting const & render_setting);

		//virtual void BindFrameBuffer(FrameBufferPtr const & fb) = 0;
		//virtual void BindSOBuffers(RenderLayoutPtr const & rl) = 0;
		//virtual void Render(ShaderObject const & shader, RenderLayout const & rl) = 0;

		virtual void Render() = 0;
		virtual void SwapBuffers() = 0;

	protected:
		std::string name_;
		RenderSetting render_setting_;

	};
}


#endif