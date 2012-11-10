#ifndef RENDERFACTORY_H_
#define RENDERFACTORY_H_

#pragma once
#include "PreDec.h"

#include "RenderEngine.h"
#include "TypeAndFormat.h"

namespace MocapGE
{
	class MOCAPGE_API RenderFactory
	{
	public:
		RenderFactory(void);
		virtual ~RenderFactory(void);

		RenderEngine& GetRenderEngine();

		virtual FrameBuffer* MakeFrameBuffer(RenderSetting& render_setting) = 0;
		virtual RenderLayout* MakeRenderLayout() = 0;	
		//make sure you you can use the same type when you call Map method of RenderBuffer
		//Only support one type of BufferUsage
		virtual RenderBuffer* MakeRenderBuffer(InitData init_data, AccessType access_type, BufferUsage usage, uint32_t width, uint32_t type_size) = 0;

		
	protected:
		RenderEngine* render_engine_;

		virtual RenderEngine* CreateRenderEngine() = 0;
	};
}

#endif