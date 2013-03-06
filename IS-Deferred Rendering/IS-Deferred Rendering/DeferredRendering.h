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
	//temp solution for pp
	class D3DShaderobject;

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

		void AddLightingBuffer( RenderView* render_view );
		void AddLightingBuffer( RenderBuffer* shader_resource);

		FrameBuffer* & GetLighingBuffer(){return lighting_buffer_;};
		RenderBuffer* & GetLightingBufferSRV(){return lighting_srv_;};

		void Update();

	private:
		//for Render Target
		FrameBuffer* gbuffer_;
		//for Shader Resource
		std::vector<RenderBuffer*> gbuffer_srv_;
		//for Lighting Buffer
		FrameBuffer* lighting_buffer_;
		RenderBuffer* lighting_srv_;

		//for shadowing
		FrameBuffer* shadowing_buffer_;
		RenderBuffer* shadowing_srv_;
		Texture* shadowing_tex_;


		Texture* shadow_blur_;

		//TODO: create own shader object
		D3DShaderobject* shadow_map_blur_so_;
		PostProcess* shadow_map_xblur_pp_;
		PostProcess* shadow_map_yblur_pp_;
		Texture* shadow_blur_X_;
		Texture* shadow_blur_Y_;

		D3DShaderobject* linearize_shadow_map_so_;
		PostProcess* linearize_shadow_map_pp_;
		FrameBuffer* shadow_map_buffer_;
		Texture* shadow_depth_;
		Texture* shadow_linear_depth_;

		//final blur texture
		RenderBuffer* shadow_blur_srv_;

		RenderBuffer* depth_srv_;




		Mesh* fullscreen_mesh_;

	};
}

#endif

