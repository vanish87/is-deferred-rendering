#ifndef TEXTURE_H_
#define TEXTURE_H_

#pragma once

#include "PreDec.h"
#include "TypeAndFormat.h"

namespace MocapGE
{
	class Texture
	{
	public:
		Texture(void);
		~Texture(void);

	protected:
		uint32_t access_flag_;
		uint32_t array_size_;
		Format format_;
		uint32_t mip_level_;
		uint32_t sample_count_;
		uint32_t sample_quality_;
		uint32_t usage_;
		uint32_t bind_flag_;
		TextureType type_;
	};
}

#endif

