#include "D3DShaderobject.h"
#include "Context.h"

namespace MocapGE
{
	D3DShaderobject::D3DShaderobject(void)
	{
	}


	D3DShaderobject::~D3DShaderobject(void)
	{
	}

	void D3DShaderobject::LoadFxoFile( std::string file_name )
	{
		std::ifstream fin(file_name, std::ios::binary);

		if (!fin)PRINT("Cannot open Fxo File ");

		fin.seekg(0, std::ios_base::end);
		int size = (int)fin.tellg();
		fin.seekg(0, std::ios_base::beg);
		std::vector<char> compiledShader(size);

		fin.read(&compiledShader[0], size);
		fin.close();
		D3DRenderEngine* render_engine = static_cast<D3DRenderEngine*>(&Context::Instance().GetRenderFactory().GetRenderEngine());
		HRESULT result = D3DX11CreateEffectFromMemory(&compiledShader[0], size,	0, render_engine->D3DDevice(), &fx_);
		if(FAILED(result))
			PRINT("Cannot Load Effect File");
	}

	void D3DShaderobject::SetVariable( std::string name )
	{
		effect_variable_[name] = fx_->GetVariableByName(name.c_str());
	}

	void D3DShaderobject::SetMatrixVariable( std::string name )
	{
		matrix_variable_[name] = fx_->GetVariableByName(name.c_str())->AsMatrix();
	}

	void D3DShaderobject::SetMatrixVariable( std::string name, float4x4 & matrix )
	{
		//TODO: write a better solution for cast float4x4 to float*
		ID3DX11EffectMatrixVariable* mat_var = GetMatrixVariable(name);
		float *p = new float[matrix.size()];
		for(size_t i = 0; i< matrix.row(); i++)
			for(size_t j = 0; j < matrix.col(); j++)
				p[i*matrix.row() + j] = matrix[i][j];
		mat_var->SetMatrix(p);
		delete[] p;
	}

	void D3DShaderobject::SetVectorVariable( std::string name )
	{
		vector_variable_[name] = fx_->GetVariableByName(name.c_str())->AsVector();
	}

	void D3DShaderobject::SetVectorVariable( std::string name, float3 & vec3 )
	{
		ID3DX11EffectVectorVariable* vec3_var = GetVectorVariable(name);
		float data[3] = {vec3[0], vec3[1], vec3[2]};
		vec3_var->SetFloatVector(data);
	}

	void D3DShaderobject::SetVectorVariable( std::string name, float4 & vec4 )
	{
		ID3DX11EffectVectorVariable* vec4_var = GetVectorVariable(name);
		float data[4] = {vec4[0], vec4[1], vec4[2], vec4[3]};
		vec4_var->SetFloatVector(data);
	}

	void D3DShaderobject::SetTechnique( std::string name )
	{
		tech_ = fx_->GetTechniqueByName(name.c_str());
	}

	size_t D3DShaderobject::GetPass()
	{
		D3DX11_TECHNIQUE_DESC tech_desc;
		tech_->GetDesc( &tech_desc );
		return tech_desc.Passes;
	}

	void D3DShaderobject::Apply( size_t pass_index )
	{
		D3DRenderEngine* d3d_render_engine = static_cast<D3DRenderEngine*>(&Context::Instance().GetRenderFactory().GetRenderEngine());
		tech_->GetPassByIndex(pass_index)->Apply(0, d3d_render_engine->D3DDeviceImmContext());
	}

	void D3DShaderobject::SetRawData( std::string name, void* data, uint32_t size )
	{
		ID3DX11EffectVariable* var = this->GetVariable(name);
		var->SetRawValue(data, 0, size);
	}

}
