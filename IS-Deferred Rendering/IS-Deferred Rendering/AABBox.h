#ifndef AABBOX_H_
#define AABBOX_H_

#pragma once
#include "Vector.h"
#include "TypeAndFormat.h"

namespace MocapGE
{
	class AABBox
	{
	public:
		AABBox(void);
		~AABBox(void);

		AABBox(VertexType* vertice, uint32_t vcout);

		void ComputeAABB(VertexType* vertice, uint32_t vcout);
		float3 Min();
		float3 Max();

	private:
		Vec3<float> min_;
		Vec3<float> max_;
	};


}
#endif
