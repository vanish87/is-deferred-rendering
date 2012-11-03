#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#pragma once
#include <vector>
#include <iterator>
#include "PreDec.h"


#include "RenderElement.h"
#include "SceneObject.h"

namespace MocapGE
{
	class SceneManager
	{
	public:
		SceneManager(void);
		~SceneManager(void);

		void AddSceneObject(SceneObject* scene_object);

		void Flush();
		void Update();

	protected:
		std::vector<RenderElement*> render_list_;
		std::vector<SceneObject*> scene_object_list;
	};

}
#endif

