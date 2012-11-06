#ifndef RENDERENGINE_H_
#define RENDERENGINE_H_

#pragma once

#include <string>

#include "PreDec.h"
#include "Configure.h"
#include "FrameBuffer.h"
#include "SceneObject.h"

namespace MocapGE
{
	class MOCAPGE_API RenderEngine
	{
	public:
		RenderEngine(void);
		~RenderEngine(void);
		virtual void InitRenderWindow(std::string const & name, RenderSetting const & render_setting);

		virtual void BindFrameBuffer(FrameBuffer* const & frame_buffer) = 0;
		virtual FrameBuffer* CurrentFrameBuffer(){return cur_frame_buffer_;};
		//virtual void BindSOBuffers(RenderLayoutPtr const & rl) = 0;
		//virtual void Render(ShaderObject const & shader, RenderLayout const & rl) = 0;

		virtual void RenderFrameBegin() = 0;
		virtual void Render(RenderLayout* render_layout, ShaderObject* shader_object) = 0;
		virtual void RenderFrameEnd() = 0;
		virtual void SwapBuffers() = 0;
		virtual void OnResize() = 0;

	protected:
		std::string name_;
		RenderSetting render_setting_;

		FrameBuffer* cur_frame_buffer_;

	};
}


#endif