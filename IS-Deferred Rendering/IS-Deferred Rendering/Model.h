#ifndef MODEL_H_
#define MODEL_H_

#pragma once

#include "PreDec.h"

#include <vector>
#include "RenderElement.h"
#include "TypeAndFormat.h"
#include "Mesh.h"
#include "Texture.h"

namespace MocapGE
{
	class Model : public RenderElement
	{
	public:
		Model(void);
		~Model(void);

		virtual void AddToScene();

		virtual RenderLayout* GetRenderLayout();

		virtual void Render();



	protected:
		std::vector<Mesh*> meshes_;
		std::vector<Material*> materials_;
		std::vector<Texture*> textures_;
	};
}

#endif

