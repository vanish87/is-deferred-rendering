//
//
//This is a temporary solution for model loader in D3D. Because it uses D3DtextureLoader tool from Microsoft.
#ifndef D3DMODEL_H_
#define D3DMODEL_H_

#pragma once

#include "PreDec.h"
#include "Model.h"

namespace MocapGE
{
	class D3DModel : public Model
	{
	public:
		D3DModel(void);
		~D3DModel(void);

		//Load D3DTexture here
		virtual void LoadFile(std::string file_name);

		virtual void SetRenderParameters();
	};

}

#endif

