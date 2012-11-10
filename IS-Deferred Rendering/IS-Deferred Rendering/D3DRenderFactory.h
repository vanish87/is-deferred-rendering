#ifndef D3DRENDERFACTORY_H_
#define D3DRENDERFACTORY_H_

#pragma once

#include "PreDec.h"
#include "Context.h"
#include "RenderFactory.h"
#include "D3DPreDec.h"

#include "D3DRenderEngine.h"
#include "D3DFrameBuffer.h"
#include "D3DRenderEngine.h"
#include "D3DRenderLayout.h"
#include "D3DRenderBuffer.h"

namespace MocapGE
{
	class MOCAPGE_API D3DRenderFactory : public RenderFactory
	{
	public:
		D3DRenderFactory(void);
		~D3DRenderFactory(void);		
		
		virtual FrameBuffer* MakeFrameBuffer( RenderSetting& render_setting );

	private:

		virtual RenderEngine* CreateRenderEngine();

		virtual RenderLayout* MakeRenderLayout();

		RenderBuffer* MakeRenderBuffer(InitData init_data, AccessType access_type, BufferUsage usage, uint32_t width, uint32_t type_size);


	};

}

#endif