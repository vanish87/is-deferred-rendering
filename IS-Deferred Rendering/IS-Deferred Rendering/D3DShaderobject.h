//
//this is temporary solution for effect file in D3D11, it will be replace by engine's own render effect class.

#ifndef D3DSHADEROBJECT_H_
#define D3DSHADEROBJECT_H_

#pragma once
#include <map>
#include <fstream>

#include "PreDec.h"
#include "D3DPreDec.h"
#include "Context.h"
#include "ShaderObject.h"
#include "D3DRenderEngine.h"

#include "..\External\D3dx11effect.h"

#pragma comment(lib, "..\\External\\Effects11.lib")
#pragma comment(lib, "d3dcompiler.lib")


namespace MocapGE
{
	class D3DShaderobject : public ShaderObject
	{
	public:
		D3DShaderobject(void);
		~D3DShaderobject(void);

		void LoadFxoFile(std::string file_name);

	private:

		ID3DX11Effect* fx_;
		ID3DX11EffectTechnique* tech_;
		std::map<std::string, ID3DX11EffectMatrixVariable*> matrix_variable_;
		std::map<std::string, ID3DX11EffectVectorVariable*> vector_variable_;
		std::map<std::string, ID3DX11EffectVariable*>		effect_variable_;
	};

}

#endif

