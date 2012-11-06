#ifndef MODEL_H_
#define MODEL_H_

#pragma once

#include <vector>

#include <dae.h>
#include <dom.h>
#include <dom/domCOLLADA.h>
#pragma comment(lib, "libcollada14dom22-d.lib")

#include "PreDec.h"

#include "RenderElement.h"
#include "RenderLayout.h"
#include "TypeAndFormat.h"
#include "Mesh.h"
#include "Texture.h"

namespace MocapGE
{
	class Model : public RenderElement
	{
		struct VertexType//temp struct for loading
		{
			float3 position;
			//float4 color;
			float3 normal;
			float2 uv;
			float3 tangent;
			float3 bitangent;
		};
		struct DaeMesh//temp struct for loading
		{
			daeElement* geometry;
			daeElement* material;
			std::vector<float3> positions;
			std::vector<float2> uvs;
			std::vector<float3> normals;
			std::vector<float3> tangents;
			std::vector<float3> bitangents;

			std::vector<std::vector<uint32_t>> indices;

			std::string name_;
			float4x4 model_matrix_;

			DaeMesh(std::string name, float4x4 model_matrix)
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
		Model(void);
		~Model(void);

		virtual void AddToScene();

		virtual RenderLayout* GetRenderLayout();

		virtual void Render();
		virtual void SetRenderParameters();
		virtual void EndRender();

		virtual void LoadFile(std::string file_name);
		void AddMesh( Mesh* mesh );
		void AddMaterial( Material* mat );
		virtual void LoadShaderFile(std::string file_name);
		

	protected:
		std::vector<Mesh*> meshes_;
		std::vector<Material*> materials_;
		std::vector<Texture*> textures_;

		float4x4 processMatrix(daeElement* node);
		void processSource( DaeMesh* mesh, daeElement* sources );
		void processTriangles( DaeMesh* mesh, daeElement* triangles );
		Material* ProcessMaterial(DaeMesh* mesh, daeElement* material);

	};
}

#endif

