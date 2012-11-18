#include "D3DModel.h"

namespace MocapGE
{
	D3DModel::D3DModel(void)
	{
		theta =0;
	} 


	D3DModel::~D3DModel(void)
	{
	}

	void D3DModel::LoadFile( std::string file_name )
	{
		Model::LoadFile(file_name);
	}

	void D3DModel::SetRenderParameters()
	{
		D3DShaderobject* d3d_shader_object = static_cast<D3DShaderobject*>(shader_object_);
		d3d_shader_object->SetMatrixVariable("g_world_matrix", model_matrix_);


		//if(deferred_rendering)
			//set parameter here
			//
	}

	void D3DModel::Render(int pass_index)
	{
		//TODO : After write a normal ShaderObject, move these to SceneManager->Flush(), because all Render_elenment shader the same lights.
		//set light parameter
		std::vector<Light*> lights = Context::Instance().GetSceneManager().GetLights();
		D3DRenderBuffer* lights_buffer = static_cast<D3DRenderBuffer*>(Context::Instance().GetRenderFactory().GetRenderEngine().GetLightsBuufer());
		shader_object_->SetReource("gLight", lights_buffer, 0);

		//for each mesh 
		for(size_t i =0; i < meshes_.size(); i++)
		{
			//set texture
			//set material
			shader_object_->SetRawData("gMaterial", materials_[i], sizeof(Material));			
			float4x4 view_mat = Context::Instance().GetRenderFactory().GetRenderEngine().CurrentFrameBuffer()->GetFrameCamera()->GetViewMatirx();
			float4x4 world_inv_transpose = Math::InverTranspose( meshes_[i]->GetModelMatrix()* model_matrix_ );
			shader_object_->SetMatrixVariable("g_world_inv_transpose", world_inv_transpose);
			//set mesh's parameter
			meshes_[i]->SetRenderParameters();
			//set mesh's texture
			if(textures_.size())
			{
				RenderBuffer* tex_srv = Context::Instance().GetRenderFactory().MakeRenderBuffer(textures_[i],AT_GPU_READ,BU_SHADER_RES);
				shader_object_->SetReource("mesh_diffuse",tex_srv, 1);
			}
			//render
			meshes_[i]->Render(pass_index);
			//end render
			meshes_[i]->EndRender();
		}		
		
	}

	void D3DModel::EndRender()
	{

	}

	void D3DModel::LoadShaderFile( std::string file_name )
	{
		shader_object_ = new D3DShaderobject();
		D3DShaderobject* d3d_shader_object = static_cast<D3DShaderobject*>(shader_object_);
		d3d_shader_object->LoadFxoFile(file_name);

		//set all meshes' shader file to this one
		for(size_t i =0; i < meshes_.size(); i++)
		{
			meshes_[i]->SetShaderObject(d3d_shader_object);
		}

		//Default init for Model shader
		//d3d_shader_object->SetTechnique("ColorTech");
		RenderEngine* render_engine = &Context::Instance().GetRenderFactory().GetRenderEngine();
		if(render_engine->GetRenderSetting().deferred_rendering)
			d3d_shader_object->SetTechnique("GbufferTech");
		d3d_shader_object->SetMatrixVariable("g_world_matrix");
		d3d_shader_object->SetMatrixVariable("g_world_inv_transpose");
		d3d_shader_object->SetMatrixVariable("g_view_proj_matrix");
		d3d_shader_object->SetMatrixVariable("g_model_matrix");
		d3d_shader_object->SetVariable("gMaterial");
		d3d_shader_object->SetShaderResourceVariable("mesh_diffuse");

		d3d_shader_object->SetVectorVariable("g_light_color");
		d3d_shader_object->SetVectorVariable("g_light_position");
		d3d_shader_object->SetVectorVariable("g_eye_pos");

		d3d_shader_object->SetShaderResourceVariable("gLight");

		
		if(render_engine->GetRenderSetting().deferred_rendering)
		{
			//init parameter here
			d3d_shader_object->SetShaderResourceVariable("position_tex");
			d3d_shader_object->SetShaderResourceVariable("normal_tex");
			d3d_shader_object->SetShaderResourceVariable("diffuse_tex");
			d3d_shader_object->SetShaderResourceVariable("specular_tex");
		}
			
	}

	Texture* D3DModel::LoadTexture( std::string file_name )
	{
		//only for load d3d Texture
		//if I have a original texture file loader, remove it, do Texture loading on Model Class
		D3DRenderEngine* d3d_re = static_cast<D3DRenderEngine*>(&Context::Instance().GetRenderFactory().GetRenderEngine());	
		ID3D11Resource* texture;
		std::wstring widestr = std::wstring(file_name.begin(), file_name.end());
		HRESULT result = DirectX::CreateWICTextureFromFile(d3d_re->D3DDevice(), NULL,
											widestr.c_str(), &texture, NULL);
		if(FAILED(result))
			PRINT("Cannot Load Texture File");
		ID3D11Texture2D* texture_2d= static_cast<ID3D11Texture2D*>(texture);
		D3D11_TEXTURE2D_DESC desc;
		texture_2d->GetDesc(&desc);
		//TODO: use unified routine to Create Texture
		D3DTexture2D* d3d_tex = new D3DTexture2D(desc,texture_2d);
		
		return d3d_tex;
		
	}


}
