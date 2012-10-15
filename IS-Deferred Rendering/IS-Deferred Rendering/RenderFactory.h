#ifndef RENDERFACTORY_H_
#define RENDERFACTORY_H_

#pragma once
#include "PreDec.h"

#include "RenderEngine.h"

namespace MocapGE
{
	class MOCAPGE_API RenderFactory
	{
	public:
		RenderFactory(void);
		virtual ~RenderFactory(void);

		RenderEngine& GetRenderEngine();

		virtual FrameBuffer* MakeFrameBuffer(RenderSetting& render_setting) = 0;

	protected:
		RenderEngine* render_engine_;

		virtual RenderEngine* CreateRenderEngine() = 0;
	};
}

#endif