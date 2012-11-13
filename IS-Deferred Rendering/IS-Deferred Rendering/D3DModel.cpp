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
		//do Load texture here
	}

	void D3DModel::SetRenderParameters()
	{
		D3DShaderobject* d3d_shader_object = static_cast<D3DShaderobject*>(shader_object_);
		d3d_shader_object->SetMatrixVariable("g_world_matrix", model_matrix_);

		//if(deferred_rendering)
			//set parameter here
			//
	}

	void D3DModel::Render()
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
			std::cout<<"Mat: "<<i<<" " <<materials_[i]->diffuse.x() <<" "<<materials_[i]->diffuse.y()<<" "<<materials_[i]->diffuse.z()<<std::endl;
			float4x4 view_mat = Context::Instance().GetRenderFactory().GetRenderEngine().CurrentFrameBuffer()->GetFrameCamera()->GetViewMatirx();
			float4x4 world_inv_transpose = Math::InverTranspose( meshes_[i]->GetModelMatrix()* model_matrix_ );
			shader_object_->SetMatrixVariable("g_world_inv_transpose", world_inv_transpose);
			//set mesh's parameter
			meshes_[i]->SetRenderParameters();
			//render
			meshes_[i]->Render();
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
		d3d_shader_object->SetTechnique("ColorTech");
		//d3d_shader_object->SetTechnique("GbufferTech");
		d3d_shader_object->SetMatrixVariable("g_world_matrix");
		d3d_shader_object->SetMatrixVariable("g_world_inv_transpose");
		d3d_shader_object->SetMatrixVariable("g_view_proj_matrix");
		d3d_shader_object->SetMatrixVariable("g_model_matrix");
		d3d_shader_object->SetVariable("gMaterial");

		d3d_shader_object->SetVectorVariable("g_light_color");
		d3d_shader_object->SetVectorVariable("g_light_position");
		d3d_shader_object->SetVectorVariable("g_eye_pos");

		d3d_shader_object->SetShaderResourceVariable("gLight");

		RenderEngine* render_engine = &Context::Instance().GetRenderFactory().GetRenderEngine();
		if(render_engine->GetRenderSetting().deferred_rendering)
		{
			//init parameter here
			d3d_shader_object->SetShaderResourceVariable("position_tex");
			d3d_shader_object->SetShaderResourceVariable("normal_tex");
			d3d_shader_object->SetShaderResourceVariable("diffuse_tex");
			d3d_shader_object->SetShaderResourceVariable("spacular_tex");
		}
			
	}


}
