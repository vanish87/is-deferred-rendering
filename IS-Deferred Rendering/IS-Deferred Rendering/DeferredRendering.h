#ifndef DEFERREDRENDERING_H_
#define DEFERREDRENDERING_H_

#pragma once

#include <vector>

#include "PreDec.h"
#include "Context.h"
#include "FrameBuffer.h"
#include "RenderLayout.h"
#include "Mesh.h"

namespace MocapGE
{
	class DeferredRendering
	{
	public:
		DeferredRendering(RenderSetting& render_setting);
		~DeferredRendering(void);

		void AddGBuffer(RenderView* render_target_view);
		void AddGBuffer(RenderBuffer* shader_resource_view);

		FrameBuffer* & GetGBuffer(){return gbuffer_;};
		std::vector<RenderBuffer*> GetGBufferSRV(){return gbuffer_srv_;};

		Mesh* GetQuadMesh(){return fullscreen_mesh_;};
		Mesh* GetFullscreenQuad();
	private:
		//for Render Target
		FrameBuffer* gbuffer_;
		//for Shader Resource
		std::vector<RenderBuffer*> gbuffer_srv_;

		Mesh* fullscreen_mesh_;

	};
}

#endif

