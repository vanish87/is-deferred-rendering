#ifndef D3DRENDERFACTORY_H_
#define D3DRENDERFACTORY_H_

#pragma once

#include "PreDec.h"
#include "RenderFactory.h"
#include "D3DPreDec.h"
#include "D3DFreamBuffer.h"
#include "D3DRenderEngine.h"

namespace MocapGE
{
	class MOCAPGE_API D3DRenderFactory : public RenderFactory
	{
	public:
		D3DRenderFactory(void);
		~D3DRenderFactory(void);

		virtual RenderEngine* CreateRenderEngine();
		
		virtual FrameBuffer* MakeFrameBuffer( RenderSetting& render_setting );

	private:

	};

}

#endif