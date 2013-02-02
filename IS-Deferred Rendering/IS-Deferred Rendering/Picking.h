#pragma once

#include <vector>
#include "PreDec.h"
#include "D3DModel.h"

class Picking
{
public:
	Picking(void);
	~Picking(void);

	//make all points in camera space
	bool GetIntersection( MocapGE::D3DModel* model, MocapGE::Viewport* viewport, MocapGE::float2 screen_point,
							MocapGE::float3& intersected_point, MocapGE::float3& intersected_normal = MocapGE::float3(0,0,0));

private:
	std::vector<std::vector<MocapGE::VertexType*>> vertice_cpu_;
	MocapGE::D3DModel* model_;
	std::vector<MocapGE::AABBox*> aabbs_;
};

