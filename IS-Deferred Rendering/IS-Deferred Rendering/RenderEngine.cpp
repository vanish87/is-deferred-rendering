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
	
}
