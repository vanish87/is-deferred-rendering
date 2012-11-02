#ifndef TYPEANDFORMAT_H_
#define TYPEANDFORMAT_H_

#pragma once

#include "PreDec.h"
#include "Vector.h"

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

	enum BufferUsage
	{
		BU_DYNAMIC,
		BU_STATIC
	};
	enum PrimitiveType
	{
		PT_POINTLIST,
		PT_TRIANGLELIST
	};
	enum VertexUsage
	{
		VU_POSITION,
		VU_NORMAL,
		VU_DIFFUSE,
		VU_SPECULAR,
		VU_TEXCOORD,
		VU_TANGENT,
		VU_BINORMAL,
		VU_BLENDINDEX,
		VU_BLENDWEIGHT
	};

	struct InitData
	{
		void const * data;
		uint32_t row_pitch;
		uint32_t slice_pitch;
	};

	struct Material
	{
		float3 ambient;
		float3 diffuse;
		float3 specular;
		float shininess;
	};

}

#endif