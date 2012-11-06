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
	}

	void D3DModel::Render()
	{

		
		//TODO : After write a normal ShaderObject, move these to SceneManager->Flush(), because all Render_elenment shader the same lights.
		//for each light
		std::vector<Light*> lights = Context::Instance().GetSceneManager().GetLights();
		for(size_t j = 0; j < lights.size(); j++)
		{
			//set light parameter
			switch (lights[j]->GetType())
			{
			case LT_POINT:
				shader_object_->SetVectorVariable("g_light_position", static_cast<PointLight*>(lights[j])->GetPos());
				break;
			case LT_SPOT:
				break;
			case LT_DERECTIONAL:
				break;
			default:
				break;
			}
			shader_object_->SetVectorVariable("g_light_color", lights[j]->GetColor());
			//for each mesh 
			for(size_t i =0; i < meshes_.size(); i++)
			{
				//set texture
				//set material
				shader_object_->SetRawData("gMaterial", materials_[i], sizeof(Material));
				float4x4 view_mat = Context::Instance().GetRenderFactory().GetRenderEngine().CurrentFrameBuffer()->GetFrameCamera()->GetViewMatirx();
				float4x4 world_inv_transpose = Math::InverTranspose( meshes_[i]->GetModelMatrix() * model_matrix_);
				shader_object_->SetMatrixVariable("g_world_inv_transpose", world_inv_transpose);
				//set mesh's parameter
				meshes_[i]->SetRenderParameters();
				//render
				meshes_[i]->Render();
				//end render
				meshes_[i]->EndRender();
			}
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
		d3d_shader_object->SetMatrixVariable("g_world_matrix");
		d3d_shader_object->SetMatrixVariable("g_world_inv_transpose");
		d3d_shader_object->SetMatrixVariable("g_view_proj_matrix");
		d3d_shader_object->SetMatrixVariable("g_model_matrix");
		d3d_shader_object->SetVariable("gMaterial");

		d3d_shader_object->SetVectorVariable("g_light_color");
		d3d_shader_object->SetVectorVariable("g_light_position");
		d3d_shader_object->SetVectorVariable("g_eye_pos");
	}


}
