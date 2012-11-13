#include "RenderEngine.h"

namespace MocapGE
{
	RenderEngine::RenderEngine(void)
	{
	}


	RenderEngine::~RenderEngine(void)
	{
	}

	void RenderEngine::InitRenderWindow( std::string const & name, RenderSetting const & render_setting )
	{
		name_ = name;
		render_setting_ = render_setting;
	}

	void RenderEngine::InitLights()
	{
		std::vector<Light*> lights = Context::Instance().GetSceneManager().GetLights();
		uint32_t size = lights.size();
		LightStruct* light = new LightStruct[size];
		for(size_t i = 0; i < size; i++)
		{
			light[i].color = lights[i]->GetColor();
			light[i].positionView = static_cast<PointLight*>(lights[i])->GetPos();
		}
		InitData init_data;
		init_data.data = light;
		init_data.row_pitch = 0;
		init_data.slice_pitch = 0;
		light_buffer_ = Context::Instance().GetRenderFactory().MakeRenderBuffer(init_data, AT_CPU_WRITE, BU_SR_SB, size , sizeof(LightStruct));
	}

	void RenderEngine::InitDeferredRendering( RenderSetting render_setting )
	{
		deferred_rendering_ = new DeferredRendering(render_setting);
		//According to config file
		for(size_t i=0; i< render_setting.gbuffer_size; i++)
		{
			//create render target
			Texture* texture_2d = Context::Instance().GetRenderFactory().MakeTexture2D(nullptr, render_setting.width, render_setting.height,
									1, 1, R32G32B32A32_F, render_setting.msaa4x ==1 ? 4 : 1, 0, AT_GPU_WRITE, TU_SR_RT);
			//Add to gbuffer
			RenderView* render_view = Context::Instance().GetRenderFactory().MakeRenderView(texture_2d, 1, 0);
			RenderBuffer* shader_resource = Context::Instance().GetRenderFactory().MakeRenderBuffer(texture_2d, AT_GPU_READ, BU_SHADER_RES);
			deferred_rendering_->AddGBuffer(render_view);
			deferred_rendering_->AddGBuffer(shader_resource);
		}
	}

	FrameBuffer* RenderEngine::GetGBuffer()
	{
		return deferred_rendering_->GetGBuffer();
	}
	std::vector<RenderBuffer*> RenderEngine::GetGBufferSRV()
	{
		return deferred_rendering_->GetGBufferSRV();
	}

	Mesh* RenderEngine::GetFullscreenQuad()
	{
		return deferred_rendering_->GetFullscreenQuad();
	}

}
