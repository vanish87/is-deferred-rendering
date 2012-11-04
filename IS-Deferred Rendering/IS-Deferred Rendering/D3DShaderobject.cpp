#include "D3DShaderobject.h"

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

}
