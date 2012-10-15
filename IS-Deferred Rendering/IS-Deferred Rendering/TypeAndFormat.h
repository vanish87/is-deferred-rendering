#ifndef TYPEANDFORMAT_H_
#define TYPEANDFORMAT_H_

#pragma once

#include "PreDec.h"

namespace MocapGE
{
	enum Format
	{
		A8_U,
		R8_U,
		R8G8B8A8_U
	};

	enum TextureType
	{
		TEXTURE1D,
		TEXTURE2D,
		TEXTURE3D
	};

	struct InitData
	{
		void const * data;
		uint32_t row_pitch;
		uint32_t slice_pitch;
	};

}

#endif