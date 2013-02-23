#include "DeferredRendering.h"

namespace MocapGE
{
	DeferredRendering::DeferredRendering( RenderSetting& render_setting )
	{
		gbuffer_ = Context::Instance().GetRenderFactory().MakeFrameBuffer(render_setting);
		lighting_buffer_ = Context::Instance().GetRenderFactory().MakeFrameBuffer(render_setting);
		//make a full screen qua for lighting pass
		VertexType* vb = new VertexType[6];
		uint32_t* ib = new uint32_t[6];
		vb[0].position = float3(-1, +1, 1);
		vb[1].position = float3(+1, +1, 1);
		vb[2].position = float3(-1, -1, 1);
		vb[3].position = float3(+1, -1, 1);
		vb[4].position = float3(-1, -1, 1);
		vb[5].position = float3(+1, +1, 1);

		ib[0] = 0;
		ib[1] = 1;
		ib[2] = 2;
		ib[3] = 3;
		ib[4] = 4;
		ib[5] = 5;

		//call MakeRenderLayout
		RenderLayout* render_layout = Context::Instance().GetRenderFactory().MakeRenderLayout();
		//call MakeRenderBuffer(Vertex)
		InitData init_data;
		init_data.data = vb;
		init_data.row_pitch = 0;
		init_data.slice_pitch = 0;
		RenderBuffer* vertex_buffer = Context::Instance().GetRenderFactory().MakeRenderBuffer(init_data, AT_GPU_READ, BU_VERTEX, 6 ,sizeof(VertexType));
		//delete[] vb;
		//call MakeRenderBuffer(Index)
		init_data.data = ib;
		init_data.row_pitch = 0;
		init_data.slice_pitch = 0;
		RenderBuffer* index_buffer = Context::Instance().GetRenderFactory().MakeRenderBuffer(init_data, AT_GPU_READ, BU_INDEX, 6, sizeof(uint32_t));
		//delete[] ib;

		//add VertexBuffer to renderlayout;
		render_layout->AddBuffer(vertex_buffer, sizeof(VertexType));
		//add IndexBuffer to renderlayout;
		render_layout->AddBuffer(index_buffer, 6);
		//set Primitivetype of renderlayout;
		render_layout->SetPrimitive(PT_TRIANGLELIST);
		//set Input layout Semi
		std::vector<VertexUsage> inputlayout;
		inputlayout.push_back(VU_POSITION);
		render_layout->SetInputLayout(inputlayout);
		float4x4 model_matrix;
		Math::Identity(model_matrix);
		fullscreen_mesh_ = new MocapGE::Mesh("full screen quad", render_layout, model_matrix, vb, ib);

		//According to config file
		for(size_t i=0; i< render_setting.gbuffer_size; i++)
		{
			//create render target
			Texture* texture_2d = Context::Instance().GetRenderFactory().MakeTexture2D(nullptr, render_setting.width, render_setting.height,
				1, 1, R32G32B32A32_F, render_setting.msaa4x ==1 ? 4 : 1, 0, AT_GPU_WRITE, TU_SR_RT);
			//Add to gbuffer
			RenderView* render_view = Context::Instance().GetRenderFactory().MakeRenderView(texture_2d, 1, 0);
			RenderBuffer* shader_resource = Context::Instance().GetRenderFactory().MakeRenderBuffer(texture_2d, AT_GPU_READ, BU_SHADER_RES);
			AddGBuffer(render_view);
			AddGBuffer(shader_resource);
		}
		//init lighting buffer
		Texture* texture_2d = Context::Instance().GetRenderFactory().MakeTexture2D(nullptr, render_setting.width, render_setting.height,
			1, 1, R32G32B32A32_F, render_setting.msaa4x ==1 ? 4 : 1, 0, AT_GPU_WRITE, TU_SR_RT);
		RenderView* render_view = Context::Instance().GetRenderFactory().MakeRenderView(texture_2d, 1, 0);
		RenderBuffer* shader_resource = Context::Instance().GetRenderFactory().MakeRenderBuffer(texture_2d, AT_GPU_READ, BU_SHADER_RES);
		AddLightingBuffer(render_view);
		AddLightingBuffer(shader_resource);

		depth_srv_ = Context::Instance().GetRenderFactory().MakeRenderBuffer(gbuffer_->GetDepthTexture(), AT_GPU_READ, BU_SHADER_RES); 		

	}


	DeferredRendering::~DeferredRendering(void)
	{
	}

	void DeferredRendering::AddGBuffer( RenderView* render_target_view )
	{
		gbuffer_->AddRenderView(render_target_view);
	}

	void DeferredRendering::AddGBuffer( RenderBuffer* shader_resource_view )
	{
		gbuffer_srv_.push_back(shader_resource_view);
	}

	Mesh* DeferredRendering::GetFullscreenQuad()
	{
		return fullscreen_mesh_;
	}

	void DeferredRendering::AddLightingBuffer( RenderView* render_view )
	{
		lighting_buffer_->AddRenderView(render_view);
	}

	void DeferredRendering::AddLightingBuffer( RenderBuffer* shader_resource)
	{
		lighting_srv_ = shader_resource;
	}

	void DeferredRendering::Update()
	{
		//do DR here
/*
			//Deferred Shading
			//---------------------------------------------------------------------------
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
			//End of Deferred Shading
			//---------------------------------------------------------------------------
*/


		RenderEngine* render_engine = &Context::Instance().GetRenderFactory().GetRenderEngine();
		std::vector<RenderElement*> render_list = Context::Instance().GetSceneManager().GetRenderList();
		//Deferred Lighting
			//pass 0
			render_engine->SetNormalState();
			//bind gbuffer
			FrameBuffer* back_buffer = render_engine->CurrentFrameBuffer();
			Camera* back_frame_camera = back_buffer->GetFrameCamera();
			gbuffer_->SetFrameCamera(back_frame_camera);
			render_engine->BindFrameBuffer(gbuffer_);

			Context::Instance().GetRenderFactory().GetRenderEngine().RenderFrameBegin();
			std::vector<RenderElement*>::iterator re;
			for(re = render_list.begin() ; re < render_list.end(); re++)
			{
				(*re)->SetRenderParameters();
				//Render to Gbuffer
				//(*re)->GetShaderObject()->Apply(0);
				(*re)->Render(0);
				(*re)->EndRender();
			}

			//pass 1
			//bind lighting buffer
			lighting_buffer_->SetFrameCamera(back_frame_camera);
			render_engine->BindFrameBuffer(lighting_buffer_);
			render_engine->SetDeferredRenderingState();
			Context::Instance().GetRenderFactory().GetRenderEngine().RenderFrameBegin();
			//set lights parameters
			std::vector<Light*> lights = Context::Instance().GetSceneManager().GetLights();
			LightStruct* light_buffer = new LightStruct[lights.size()];
			/*RenderBuffer* lights_buffer = Context::Instance().GetRenderFactory().GetRenderEngine().GetLightsBuufer();
			LightStruct* l = static_cast<LightStruct*>(lights_buffer->Map(AT_CPU_WRITE));
			for (size_t i =0; i< lights.size(); i++)
			{
				l[i].color = lights[i]->GetColor();
				l[i].position = static_cast<PointLight*>(lights[i])->GetPos();
			}
			lights_buffer->UnMap();*/
			//set gbuffer as input textures

			ShaderObject* shader_object = render_list[0]->GetShaderObject();
			for (size_t i =0; i< lights.size(); i++)
			{

				light_buffer[i].color = lights[i]->GetColor();

				switch (lights[i]->GetType())
				{
				case LT_POINT:
					{
						light_buffer[i].type = LT_POINT;
						light_buffer[i].position = static_cast<PointLight*>(lights[i])->GetPos();
						light_buffer[i].direction = float3(0, 0, 0);
						light_buffer[i].inner_outer = float2(0, 0);
						break;
					}
				case LT_SPOT:
					{
						light_buffer[i].type = LT_SPOT;
						light_buffer[i].position = static_cast<SpotLight*>(lights[i])->GetPos();
						light_buffer[i].direction = static_cast<SpotLight*>(lights[i])->GetDir();
						float outer = static_cast<SpotLight*>(lights[i])->GetOuterAngle();
						float inner = static_cast<SpotLight*>(lights[i])->GetInnerAngle();
						light_buffer[i].inner_outer = float2(Math::Cos(inner), Math::Cos(outer));
						break;
					}
				default:
					break;
				}
				//LightStruct* l = &light_buffer[i];
				shader_object->SetRawData("light", &light_buffer[i], sizeof(LightStruct));
				/*

				D3DShaderobject* d3d_shader_obj = new D3DShaderobject();
				d3d_shader_obj->LoadFxoFile("..\\FxFiles\\PostProcess.fxo");
				d3d_shader_obj->SetTechnique("PPTech");
				PostProcess* pp_= new PostProcess();
				pp_->SetPPShader(d3d_shader_obj);
				pp_->SetInput(gbuffer->GetDepthTexture(), 0);
				pp_->SetOutput(gbuffer->GetDepthTexture(), 0);
				pp_->Apply();*/

				//depth_srv_ = Context::Instance().GetRenderFactory().MakeRenderBuffer(gbuffer_->GetDepthTexture(), AT_GPU_READ, BU_SHADER_RES); 			
				shader_object->SetReource("depth_tex", depth_srv_, 1);
				shader_object->SetReource("normal_tex", gbuffer_srv_[0], 1);

				//shader_object->SetReource("lighting_tex", lighting_srv_, 1);
				//do lighting
				//Set Shader file for quad
				fullscreen_mesh_->SetShaderObject(shader_object);
				fullscreen_mesh_->SetRenderParameters();
				//quad->GetShaderObject()->Apply(1);
				fullscreen_mesh_->Render(1);
				fullscreen_mesh_->EndRender();
			}

			delete[] light_buffer;

			//pass 2
			render_engine->BindFrameBuffer(back_buffer);
			Context::Instance().GetRenderFactory().GetRenderEngine().RenderFrameBegin();
			shader_object->SetReource("lighting_tex", lighting_srv_, 1);
			shader_object->SetReource("diffuse_tex", gbuffer_srv_[1], 1);
			//Set Shader file for quad
			fullscreen_mesh_->SetShaderObject(shader_object);
			fullscreen_mesh_->SetRenderParameters();
			//quad->GetShaderObject()->Apply(1);
			fullscreen_mesh_->Render(2);
			fullscreen_mesh_->EndRender();




			Context::Instance().GetRenderFactory().GetRenderEngine().RenderFrameEnd();
			Context::Instance().GetRenderFactory().GetRenderEngine().SwapBuffers();
	}


}
