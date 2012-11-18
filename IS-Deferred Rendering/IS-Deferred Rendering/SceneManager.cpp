#include "SceneManager.h"
#include "Context.h"

namespace MocapGE
{

	SceneManager::SceneManager(void)
	{
	}


	SceneManager::~SceneManager(void)
	{
	}

	void SceneManager::Flush()
	{

		RenderEngine* render_engine = &Context::Instance().GetRenderFactory().GetRenderEngine();
		if(render_engine->GetRenderSetting().deferred_rendering)
		{	//do DR here
			render_engine->SetNormalState();
			//bind gbuffer
			FrameBuffer* back_buffer = render_engine->CurrentFrameBuffer();
			Camera* back_frame_camera = back_buffer->GetFrameCamera();
			render_engine->GetGBuffer()->SetFrameCamera(back_frame_camera);
			render_engine->BindFrameBuffer(render_engine->GetGBuffer());

			Context::Instance().GetRenderFactory().GetRenderEngine().RenderFrameBegin();
			std::vector<RenderElement*>::iterator re;
			for(re = render_list_.begin() ; re < render_list_.end(); re++)
			{
				(*re)->SetRenderParameters();
				//Render to Gbuffer
				//(*re)->GetShaderObject()->Apply(0);
				(*re)->Render(0);
				(*re)->EndRender();
			}

			
			//bind screen buffer
			render_engine->BindFrameBuffer(back_buffer);
			render_engine->SetDeferredRenderingState();
			Context::Instance().GetRenderFactory().GetRenderEngine().RenderFrameBegin();
			//set lights parameters
			std::vector<Light*> lights = Context::Instance().GetSceneManager().GetLights();
			RenderBuffer* lights_buffer = Context::Instance().GetRenderFactory().GetRenderEngine().GetLightsBuufer();
			LightStruct* l = static_cast<LightStruct*>(lights_buffer->Map(AT_CPU_WRITE));
			for (size_t i =0; i< lights.size(); i++)
			{
				l[i].color = lights[i]->GetColor();
				l[i].position = static_cast<PointLight*>(lights[i])->GetPos();
			}
			lights_buffer->UnMap();

			//set gbuffer as input textures
			ShaderObject* shader_object = render_list_[0]->GetShaderObject();
			std::vector<RenderBuffer*> gbuffer_srv = render_engine->GetGBufferSRV();	
			FrameBuffer* gbuffer = render_engine->GetGBuffer();
			RenderBuffer* depth_srv = Context::Instance().GetRenderFactory().MakeRenderBuffer(gbuffer->GetDepthTexture(), AT_GPU_READ, BU_SHADER_RES); 
			shader_object->SetReource("position_tex", gbuffer_srv[3], 1);
			shader_object->SetReource("diffuse_tex", gbuffer_srv[1], 1);
			shader_object->SetReource("specular_tex", gbuffer_srv[2], 1);
			shader_object->SetReource("normal_tex", gbuffer_srv[0], 1);
			//do lighting
			Mesh* quad = render_engine->GetFullscreenQuad();
			//Set Shader file for quad
			quad->SetShaderObject(shader_object);
			quad->SetRenderParameters();
			//quad->GetShaderObject()->Apply(1);
			quad->Render(1);
			quad->EndRender();

			Context::Instance().GetRenderFactory().GetRenderEngine().RenderFrameEnd();
			Context::Instance().GetRenderFactory().GetRenderEngine().SwapBuffers();
		}	
		else
		{
			//do FR
			//Forward Rendering
			//=======================================================================================================================
			//set lights parameters
			std::vector<Light*> lights = Context::Instance().GetSceneManager().GetLights();
			RenderBuffer* lights_buffer = Context::Instance().GetRenderFactory().GetRenderEngine().GetLightsBuufer();
			LightStruct* l = static_cast<LightStruct*>(lights_buffer->Map(AT_CPU_WRITE));
			for (size_t i =0; i< lights.size(); i++)
			{
				l[i].color = lights[i]->GetColor();
				l[i].position = static_cast<PointLight*>(lights[i])->GetPos();
			}
			lights_buffer->UnMap();

			render_engine->BindFrameBuffer(render_engine->CurrentFrameBuffer());
			Context::Instance().GetRenderFactory().GetRenderEngine().RenderFrameBegin();
			std::vector<RenderElement*>::iterator re;
			for(re = render_list_.begin() ; re < render_list_.end(); re++)
			{
				(*re)->SetRenderParameters();
				//(*re)->GetShaderObject()->Apply(0);
				(*re)->Render(0);
				(*re)->EndRender();
			}
			Context::Instance().GetRenderFactory().GetRenderEngine().RenderFrameEnd();
			Context::Instance().GetRenderFactory().GetRenderEngine().SwapBuffers();
			//Forward Rendering End
			//========================================================================================================================
		}

	}

	void SceneManager::Update()
		//do scene cull here
	{
		render_list_.clear();
		std::vector<SceneObject*>::iterator so;
		for(so = scene_object_list.begin() ; so < scene_object_list.end(); so++)
		{
			(*so)->Update();//do update aabb and set model_matrix equals to its render_element_'s
			render_list_.push_back((*so)->GetRenderElement());
		}
	}

	void SceneManager::AddSceneObject( SceneObject* scene_object )
	{
		scene_object_list.push_back(scene_object);
	}

	std::vector<Light*> SceneManager::GetLights()
	{
		return lights_;
	}

	void SceneManager::AddLight( Light* light )
	{
		lights_.push_back(light);
	}

}
