//
//this is temporary solution for effect file in D3D11, it will be replace by engine's own render effect class.

#ifndef D3DSHADEROBJECT_H_
#define D3DSHADEROBJECT_H_

#pragma once

#include "PreDec.h"
#include "D3DPreDec.h"

#include "..\External\D3dx11effect.h"


namespace MocapGE
{
	class D3DShaderobject
	{
	public:
		D3DShaderobject(void);
		~D3DShaderobject(void);

	private:

		ID3DX11Effect* mFX;
		ID3DX11EffectTechnique* mTech;
	};

}

#endif

