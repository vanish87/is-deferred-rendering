#ifndef RESOURCELOADER_H_
#define RESOURCELOADER_H_

#pragma once

#include <vector>

#include <dae.h>
#include <dom.h>
#include <dom/domCOLLADA.h>
#pragma comment(lib, "libcollada14dom22-d.lib")
// #pragma comment(lib, "libxml2_a.lib")
// #pragma comment(lib, "zlib.lib")
// #pragma comment(lib, "wsock32.lib")
// #pragma comment(lib, "pcred.lib")
// #pragma comment(lib, "pcrecppd.lib")
// #pragma comment(lib, "minizip-d.lib")

#include "PreDec.h"
#include "Vector.h"
#include "Model.h"
#include "RenderLayout.h"

namespace MocapGE
{
	class ResourceLoader
	{
		struct VertexType
		{
			float3 position;
			float2 uv;
			float3 normal;
			float3 tangent;
			float3 bitangent;
		};
		struct Mesh
		{
			daeElement* geometry;

			std::vector<float3> positions;
			std::vector<float2> uvs;
			std::vector<float3> normals;
			std::vector<float3> tangents;
			std::vector<float3> bitangents;
			
			std::vector<std::vector<uint32_t>> indices;

			std::string name_;
			float4x4 model_matrix_;

			Mesh(std::string name, float4x4 model_matrix)
				:name_(name), model_matrix_(model_matrix){};
			
			size_t GetByteWidth()
			{
				return sizeof(float3)*4 + sizeof(float2);
			}

			uint32_t GetNumVertex()
			{
				return positions.size();
			}
			uint32_t GetNumIndex()
			{
				return indices[0].size();
			}
		};

	public:
		ResourceLoader(void);
		~ResourceLoader(void);

		Model* LoadModel(std::string file_name);
	private:

		float4x4 ResourceLoader::processMatrix(daeElement* node);
		void processSource( Mesh* mesh, daeElement* sources );
		void processTriangles( Mesh* mesh, daeElement* triangles );
	};
}

#endif

