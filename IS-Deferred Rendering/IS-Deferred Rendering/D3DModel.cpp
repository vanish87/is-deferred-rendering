#include "D3DModel.h"

namespace MocapGE
{
	D3DModel::D3DModel(void)
	{
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
		//TODO: write a better solution for cast float4x4 to float*
		float3 pos = float3(3.5,3.5,-3.5);
		float4x4 view_matrix = Math::LookAtLH(pos ,float3(0,0,0),float3(0,1,0));
		float4x4 proj_matrix = Math::PerspectiveFovLH(3.14f/4, 1280/800.0f, 1.0f, 1000.0f);
		Math::Identity(model_matrix_);
		model_matrix_ = model_matrix_*view_matrix*proj_matrix;
		d3d_shader_object->SetMatrixVariable("gWorldViewProj", model_matrix_);
	
	}

	void D3DModel::Render()
	{
			//for each mesh 
			for(size_t i =0; i < meshes_.size(); i++)
			{
				//set texture
				//set material
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
		d3d_shader_object->SetMatrixVariable("gWorldViewProj");
	}


}
