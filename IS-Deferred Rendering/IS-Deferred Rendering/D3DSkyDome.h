#ifndef D3DSKYDOME_H_
#define D3DSKYDOME_H_

#pragma once

#include "PreDec.h"
#include "D3DPreDec.h"
#include "Model.h"

#include "..\External\DDSTextureLoader.h"

#pragma comment(lib, "..\\External\\DirectXTK.lib")

namespace MocapGE
{
	class D3DSkyDome : public Model
	{
	public:
		D3DSkyDome(void);
		~D3DSkyDome(void);

		virtual Texture* LoadTexture(std::string file_name);

	private:
		Texture* cube_texture_;
	};

}

#endif

