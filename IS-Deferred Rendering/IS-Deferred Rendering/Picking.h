#pragma once

#include <vector>
#include "PreDec.h"
#include "D3DModel.h"

class Picking
{
public:
	Picking(void);
	Picking(MocapGE::D3DModel* model);
	~Picking(void);

	bool GetIntersection( MocapGE::D3DModel* model, MocapGE::Viewport* viewport, MocapGE::float2 screen_point, MocapGE::float3& intersected_point);

private:
	std::vector<MocapGE::VertexType*> vertice_cpu;
	MocapGE::D3DModel* model_;
	MocapGE::float4x4 model_matrix_;
	MocapGE::float4x4 world_matrix_;
	size_t vsize_;
};

