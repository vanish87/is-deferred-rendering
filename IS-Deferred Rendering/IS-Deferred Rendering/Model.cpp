#include "Model.h"
#include "Context.h"

namespace MocapGE
{
	Model::Model(void)
	{
	}


	Model::~Model(void)
	{
	}

	void Model::AddToScene()
	{
		Context::Instance().GetSceneManager().AddSceneObject(new SceneObject(this));
	}

	RenderLayout* Model::GetRenderLayout()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void Model::Render(int pass_index)
	{
		//render each mesh
		throw std::exception("The method or operation is not implemented.");
	}

	void Model::SetRenderParameters()
	{
		//only do the general setup
		throw std::exception("The method or operation is not implemented.");
	}

	void Model::EndRender()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void Model::LoadFile( std::string file_name )
	{
		std::vector<DaeMesh*> meshes;
		//Dae file
		DAE dae;

		//Root node
		daeElement* root;

		//<library_visual_scenes> node
		daeElement* library_visual_scenes;

		//<library_geometries> node
		daeElement* library_geometries;

		root = dae.open(file_name);

		if(!root)
		{
			PRINT("Cannot open DAE file");
			return ;
		}

		library_visual_scenes = root->getDescendant("library_visual_scenes");

		if(!library_visual_scenes)
		{
			PRINT( "<library_visual_scenes> not found." );
			return ;
		}

		library_geometries = root->getDescendant("library_geometries");
		if(!library_geometries)
		{
			PRINT( "<library_geometries> not found.");
			return ;
		}
		//Get the <visual_scene> node
		daeElement* visual_scene = library_visual_scenes->getDescendant("visual_scene");

		//Get all the <node>'s for the <visual_scene>
		daeTArray<daeElementRef> nodes = visual_scene->getChildren();

		for(unsigned int i = 0; i < nodes.getCount(); i++)
		{
			//Get the name and the type, if they exist
			std::string Name = nodes[i]->getAttribute("name").data();
			std::string Type = nodes[i]->getAttribute("type").data();

			//Skip JOINT node's, only meshes
			if(Type == "JOINT") continue;

			//Get the <instance_geometry> node that corresponds to this <node>
			domInstance_geometry* instance_geometry = NULL;
			domInstance_material* instance_material = NULL;
			instance_geometry = (domInstance_geometry*)nodes[i]->getDescendant("instance_geometry");
			instance_material = (domInstance_material*)nodes[i]->getDescendant("instance_material");

			//If there is no <instance_geometry>, this isn't a static mesh and we will skip it.
			if(!instance_geometry) continue;

			//Get the <geometry> node that is referenced by the <instance_geometry>
			daeElement* geometry = instance_geometry->getUrl().getElement();
			daeElement* material = instance_material->getTarget().getElement();

			//If the referenced node was not found, skip this node
			if(!geometry) continue;

			//Now create a new mesh, set it's <geometry> node and get it's World transform.
			meshes.push_back(new DaeMesh(Name, processMatrix(nodes[i]->getDescendant("matrix"))));
			meshes.back()->geometry = geometry;
			meshes.back()->material = material;
		}
		
		//Foreach mesh...
		for(unsigned int i = 0; i < meshes.size(); i++)
		{
			Material* mat = ProcessMaterial(meshes[i], meshes[i]->material);
			//Get the <mesh> node
			daeElement* mesh = meshes[i]->geometry->getDescendant("mesh");
			//Get the <source> nodes
			daeTArray<daeElementRef> sources = mesh->getChildren();


			//Process each <source> child
			int count = sources.getCount();
			for(unsigned int z = 0; z < count; z++) 
			{
				if(std::string(sources[z]->getElementName()) == "source")
				{
					processSource(meshes[i], sources[z]);
					continue;
				}
				if (std::string(sources[z]->getElementName()) == "triangles")
				{
					processTriangles(meshes[i],sources[z]);
					continue;
				}
				
			}
			

			//realign the index of vertex
			uint32_t i_size = meshes[i]->GetNumIndex();
			uint32_t v_size = i_size;
			//uint32_t v_size = meshes[i]->GetNumVertex();
			/*//===============================================================
			i_size = 36;
			v_size = 8;
			VertexType vb[] = {
			{ float3(-1.0f, -1.0f, -1.0f ),float4(1.0f, 0.0f, 0.0f ,1.0f)  },
			{ float3(-1.0f, +1.0f, -1.0f), float4(1.0f, 1.0f, 0.0f ,1.0f)  },
			{ float3(+1.0f, +1.0f, -1.0f), float4(1.0f, 1.0f, 1.0f ,1.0f)  },
			{ float3(+1.0f, -1.0f, -1.0f), float4(0.0f, 0.0f, 0.0f ,1.0f)  },
			{ float3(-1.0f, -1.0f, +1.0f), float4(0.0f, 1.0f, 0.0f ,1.0f)  },
			{ float3(-1.0f, +1.0f, +1.0f), float4(1.0f, 1.0f, 1.0f ,1.0f)  },
			{ float3(+1.0f, +1.0f, +1.0f), float4(1.0f, 0.0f, 1.0f ,1.0f)  },
			{ float3(+1.0f, -1.0f, +1.0f), float4(1.0f, 0.0f, 0.0f ,1.0f)  }
			};

			uint32_t ib[] = 	{
				// front face
				0, 1, 2,
				0, 2, 3,

				// back face
				4, 6, 5,
				4, 7, 6,

				// left face
				4, 5, 1,
				4, 1, 0,

				// right face
				3, 2, 6,
				3, 6, 7,

				// top face
				1, 5, 6,
				1, 6, 2,

				// bottom face
				4, 0, 3, 
				4, 3, 7
			};
			
			//========================================================*/
			VertexType* vb = new VertexType[v_size];
			uint32_t* ib = new uint32_t[i_size];
			for(size_t j = 0; j < i_size; j++)
			{
				float3 position=meshes[i]->positions[meshes[i]->indices[0][j]];			
				float3 normal=meshes[i]->normals[meshes[i]->indices[1][j]];

				float2 uv=float2(0,0);
				float3 tangent=float3(0,0,0);
				float3 bitangent=float3(0,0,0);
				if(meshes[i]->indices.size() > 2 )
				{
					uv=meshes[i]->uvs[meshes[i]->indices[2][j]];
					tangent=meshes[i]->tangents[meshes[i]->indices[3][j]];
					if(meshes[i]->indices.size() > 4 )
						bitangent=meshes[i]->bitangents[meshes[i]->indices[4][j]];
				}

				vb[j].position = position;
				vb[j].normal = normal;
				vb[j].uv = uv;
				vb[j].tangent = tangent;
				vb[j].bitangent = bitangent;
				ib[j] = j;
				//std::cout<<"position "<<j <<std::endl;
				//std::cout<<vb[j].position.x()<<" "<<vb[j].position.y()<<" "<<vb[j].position.z()<<std::endl;

				//std::cout<<"normal"<<std::endl;
				//std::cout<<vb[j].normal.x()<<" "<<vb[j].normal.y()<<" "<<vb[j].normal.z()<<std::endl;
				//Deprecated problem code
				/*
				vb[meshes[i]->indices[0][j]].position = position;
				vb[meshes[i]->indices[0][j]].normal = normal;
				
				std::cout<<"position"<<std::endl;
				std::cout<<vb[meshes[i]->indices[0][j]].position.x()<<" "<<vb[meshes[i]->indices[0][j]].position.y()<<" "<<vb[meshes[i]->indices[0][j]].position.z()<<std::endl;
				std::cout<<"normal"<<std::endl;
				std::cout<<vb[meshes[i]->indices[0][j]].normal.x()<<" "<<vb[meshes[i]->indices[0][j]].normal.y()<<" "<<vb[meshes[i]->indices[0][j]].normal.z()<<std::endl;
				vb[meshes[i]->indices[0][j]].uv = uv;
				vb[meshes[i]->indices[0][j]].tangent = tangent;
				vb[meshes[i]->indices[0][j]].bitangent = bitangent;
				ib[j] = meshes[i]->indices[0][j];
				*/
			}
			
			//call MakeRenderLayout
			RenderLayout* render_layout = Context::Instance().GetRenderFactory().MakeRenderLayout();
			//call MakeRenderBuffer(Vertex)
			InitData init_data;
			init_data.data = vb;
			init_data.row_pitch = 0;
			init_data.slice_pitch = 0;
			RenderBuffer* vertex_buffer = Context::Instance().GetRenderFactory().MakeRenderBuffer(init_data, AT_GPU_READ, BU_VERTEX, v_size ,sizeof(VertexType));
			//delete[] vb;
			//call MakeRenderBuffer(Index)
			init_data.data = ib;
			init_data.row_pitch = 0;
			init_data.slice_pitch = 0;
			RenderBuffer* index_buffer = Context::Instance().GetRenderFactory().MakeRenderBuffer(init_data, AT_GPU_READ, BU_INDEX, i_size, sizeof(uint32_t));
			//delete[] ib;
			//add VertexBuffer to renderlayout;
			render_layout->AddBuffer(vertex_buffer, sizeof(VertexType));
			//add IndexBuffer to renderlayout;
			render_layout->AddBuffer(index_buffer, i_size);
			//set Primitivetype of renderlayout;
			render_layout->SetPrimitive(PT_TRIANGLELIST);
			//set Input layout Semi
			std::vector<VertexUsage> inputlayout;
			inputlayout.push_back(VU_POSITION);
			//inputlayout.push_back(VU_COLOR);
			inputlayout.push_back(VU_NORMAL);
			inputlayout.push_back(VU_TEXCOORD);
			inputlayout.push_back(VU_TANGENT);
			inputlayout.push_back(VU_BINORMAL);
			render_layout->SetInputLayout(inputlayout);
			//add renderlayout to model;
			this->AddMesh(new MocapGE::Mesh(meshes[i]->name_, render_layout, meshes[i]->model_matrix_));
			this->AddMaterial(mat);
		}

		dae.close(file_name);

		Math::Identity(model_matrix_);

	}



	float4x4 Model::processMatrix(daeElement* mat_node)
	{
		float4x4 res;
		std::string model = mat_node->getCharData();
		std::stringstream stm(model);

		//LH
		stm >> res[0][0];
		stm >> res[1][0];
		stm >> res[2][0];
		stm >> res[3][0];

		stm >> res[0][1];
		stm >> res[1][1];
		stm >> res[2][1];
		stm >> res[3][1];

		stm >> res[0][2];
		stm >> res[1][2];
		stm >> res[2][2];
		stm >> res[3][2];

		stm >> res[0][3];
		stm >> res[1][3];
		stm >> res[2][3];
		stm >> res[3][3];
		
		//RH
		/*
		stm >> res[0][0];
		stm >> res[0][1];
		stm >> res[0][2];
		stm >> res[0][3];

		stm >> res[1][0];
		stm >> res[1][1];
		stm >> res[1][2];
		stm >> res[1][3];

		stm >> res[2][0];
		stm >> res[2][1];
		stm >> res[2][2];
		stm >> res[2][3];

		stm >> res[3][0];
		stm >> res[3][1];
		stm >> res[3][2];
		stm >> res[3][3];
		*/

		return res;
	}

	void Model::processSource( DaeMesh* mesh, daeElement* source )
	{
		//Get Positions
		if(source->getAttribute("id").find("positions") != std::string::npos)
		{
			//Get the <float_array> node
			daeElement* float_array = source->getChild("float_array");

			//Get the number of raw float's contained
			unsigned int count = atoi(float_array->getAttribute("count").data());

			//Get the raw string representation
			std::string positionArray = float_array->getCharData();

			//Set up a stringstream to read from the raw array
			std::stringstream stm(positionArray);

			//Read each float, in groups of three
			for(unsigned int i = 0; i < (count / 3); i++)
			{
				float x, y, z;

				stm >> x;
				stm >> y;
				stm >> z;

				//Push this back as another Position component
				mesh->positions.push_back(float3(x, y, z));
			}

			return;
		}





		//Get Tangents at layer0, the reason there are different naming schemes, this covers the ones I've come into contact with
		if(source->getAttribute("id").find("map1-textangents") != std::string::npos || source->getAttribute("id").find("textangents") != std::string::npos)
		{
			//Get the <float_array> node
			daeElement* float_array = source->getChild("float_array");

			//Get the number of raw float's contained
			unsigned int count = atoi(float_array->getAttribute("count").data());

			//Get the raw string representation
			std::string tangentsArray = float_array->getCharData();

			//Set up a stringstream to read from the raw array
			std::stringstream stm(tangentsArray);

			//Read each float, in groups of three
			for(unsigned int i = 0; i < (count / 3); i++)
			{
				float x, y, z;

				stm >> x;
				stm >> y;
				stm >> z;

				//Push this back as another Position component
				mesh->tangents.push_back(float3(x, y, z));
			}

			return;
		}

		//Get BiTangents at layer0, read above about the different names
		if(source->getAttribute("id").find("map1-texbinormals") != std::string::npos || source->getAttribute("id").find("texbinormals") != std::string::npos)
		{
			//Get the <float_array> node
			daeElement* float_array = source->getChild("float_array");

			//Get the number of raw float's contained
			unsigned int count = atoi(float_array->getAttribute("count").data());

			//Get the raw string representation
			std::string biTangentsArray = float_array->getCharData();

			//Set up a stringstream to read from the raw array
			std::stringstream stm(biTangentsArray);

			//Read each float, in groups of three
			for(unsigned int i = 0; i < (count / 3); i++)
			{
				float x, y, z;

				stm >> x;
				stm >> y;
				stm >> z;

				//Push this back as another Position component
				mesh->bitangents.push_back(float3(x, y, z));
			}

			return;
		}

		//Get UVs at layer0
		if(source->getAttribute("id").find("map1") != std::string::npos||
			source->getAttribute("id").find("UV") != std::string::npos)
		{
			//Get the <float_array> node
			daeElement* float_array = source->getChild("float_array");

			//Get the number of raw float's contained
			unsigned int count = atoi(float_array->getAttribute("count").data());

			//Get the raw string representation
			std::string uvArray = float_array->getCharData();

			//Set up a stringstream to read from the raw array
			std::stringstream stm(uvArray);

			//Read each float, in groups of three
			for(unsigned int i = 0; i < (count / 2); i++)
			{
				float x, y;

				stm >> x;
				stm >> y;

				//Push this back as another Position component
				mesh->uvs.push_back(float2(x, y));
			}

			return;
		}

		//Get Normals
		if(source->getAttribute("id").find("normals") != std::string::npos)
		{
			//Get the <float_array> node
			daeElement* float_array = source->getChild("float_array");

			//Get the number of raw float's contained
			unsigned int count = atoi(float_array->getAttribute("count").data());

			//Get the raw string representation
			std::string normalsArray = float_array->getCharData();

			//Set up a stringstream to read from the raw array
			std::stringstream stm(normalsArray);

			//Read each float, in groups of three
			for(unsigned int i = 0; i < (count / 3); i++)
			{
				float x, y, z;

				stm >> x;
				stm >> y;
				stm >> z;

				//Push this back as another Position component
				mesh->normals.push_back(float3(x, y, z));
			}

			return;
		}
	}

	void Model::processTriangles( DaeMesh* mesh, daeElement* triangles )
	{
		daeTArray<daeElementRef> input = triangles->getChildren();

		std::vector<int> offset;
		for(int i=0;i<input.getCount();i++)
		{
			if(std::string(input[i]->getElementName()).find("input")!= std::string::npos)
			{
				int os=atoi(input[i]->getAttribute("offset").data());
				offset.push_back(os);
			}

		}

		//Get the <p> node
		daeElement* p = triangles->getDescendant("p");

		//Get the number of faces, multiply by 3 to get number of indices
		unsigned int count = atoi(triangles->getAttribute("count").data()) * 3;

		//Get the raw string representation
		std::string pArray = p->getCharData();

		//Set up a stringstream to read from the raw array
		std::stringstream stm(pArray);

		//Read each unsigned int
		mesh->indices.resize(offset.size());
		for(unsigned int i = 0; i < count; i++)
		{
			unsigned int p = 0;
			for(int j=0;j<offset.size();j++)
			{
				if(offset[j]==j)
					stm >> p;
				mesh->indices[j].push_back(p);
			}
		}
	}

	Material* Model::ProcessMaterial( DaeMesh* mesh, daeElement* material )
	{
		domInstance_effect* instance_effect = (domInstance_effect*)material->getDescendant("instance_effect");
		daeElement* effect = instance_effect->getUrl().getElement();
		//std::string e_id = effect->getID();
		daeElement* phong = effect->getDescendant("phong");
		Material* material_out = new Material();

		if(!phong) 
		{
			phong = effect->getDescendant("lambert");
			//TODO : use material attribute list to read phong;
			daeElement* ambient = phong->getDescendant("ambient")->getDescendant("color");
			std::string ambient_color = ambient->getCharData();
			std::stringstream astm(ambient_color);
			astm >> material_out->ambient.x();
			astm >> material_out->ambient.y();
			astm >> material_out->ambient.z();
			astm >> material_out->ambient.w();

			daeElement* diffuse = phong->getDescendant("diffuse")->getDescendant("color");
			std::string diffuse_color = diffuse->getCharData();
			std::stringstream dstm(diffuse_color);
			dstm >> material_out->diffuse.x();
			dstm >> material_out->diffuse.y();
			dstm >> material_out->diffuse.z();
			dstm >> material_out->diffuse.w();

			material_out->specular	= float4(0 ,0 , 0, 1);
			material_out->shininess = 1;
		}
		else
		{
			//TODO : use material attribute list to read phong;
			daeElement* ambient = phong->getDescendant("ambient")->getDescendant("color");
			if(!ambient)
				material_out->ambient =float4(0.5, 0.5, 0.5, 1);
			else
			{
				std::string ambient_color = ambient->getCharData();
				std::stringstream astm(ambient_color);
				astm >> material_out->ambient.x();
				astm >> material_out->ambient.y();
				astm >> material_out->ambient.z();
				astm >> material_out->ambient.w();
			}

			daeElement* diffuse = phong->getDescendant("diffuse")->getDescendant("color");
			if(!diffuse)
				material_out->diffuse =float4(0, 0, 0, 1);
			else
			{
				std::string diffuse_color = diffuse->getCharData();
				std::stringstream dstm(diffuse_color);
				dstm >> material_out->diffuse.x();
				dstm >> material_out->diffuse.y();
				dstm >> material_out->diffuse.z();
				dstm >> material_out->diffuse.w();
			}

			daeElement* specular = phong->getDescendant("specular")->getDescendant("color");
			if(!specular)
				material_out->specular =float4(0, 0, 0, 1);
			else
			{
				std::string specular_color = specular->getCharData();
				std::stringstream sstm(specular_color);
				sstm >> material_out->specular.x();
				sstm >> material_out->specular.y();
				sstm >> material_out->specular.z();
				sstm >> material_out->specular.w();
			}

			daeElement* shininess = phong->getDescendant("shininess")->getDescendant("float");
			if(!shininess)
				material_out->shininess = 1;
			else
			{
				std::string shininess_f = shininess->getCharData();
				std::stringstream shstm(shininess_f);
				shstm >> material_out->shininess;
			}
		}
		

		return material_out;
	}


	void Model::AddMesh( Mesh* mesh )
	{
		meshes_.push_back(mesh);
	}
	void Model::AddMaterial( Material* mat )
	{
		materials_.push_back(mat);
	}

	void Model::LoadShaderFile( std::string file_name )
	{

	}	

}