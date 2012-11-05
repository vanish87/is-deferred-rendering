//
//
//TODO@This is a temporary solution for model loader in D3D. Because it uses D3DtextureLoader tool from Microsoft.
#ifndef D3DMODEL_H_
#define D3DMODEL_H_

#pragma once

#include "PreDec.h"
#include "D3DPreDec.h"
#include "Model.h"

#include "D3DShaderobject.h"

namespace MocapGE
{
	//Use LoadFile to load .dae file
	//Use LoadShaderFile to load .fxo file, which is the compiled file of .fx file
	class D3DModel : public Model
	{
	public:
		D3DModel(void);
		~D3DModel(void);

		//Load .dae file
		virtual void LoadFile(std::string file_name);
		//use this to load .fxo file only
		virtual void LoadShaderFile(std::string file_name);

		virtual void SetRenderParameters();
		virtual void Render();
		virtual void EndRender();
	};

}

#endif

