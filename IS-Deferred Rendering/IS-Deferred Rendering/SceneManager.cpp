#include "SceneManager.h"

namespace MocapGE
{

	SceneManager::SceneManager(void)
	{
	}


	SceneManager::~SceneManager(void)
	{
	}

	void SceneManager::Flush()
	{
		std::vector<RenderElement*>::iterator re;
		for(re = render_list_.begin() ; re < render_list_.end(); re++)
			(*re)->Render();
	}

	void SceneManager::Update()
	{
		std::vector<SceneObject*>::iterator so;
		for(so = scene_object_list.begin() ; so < scene_object_list.end(); so++)
			(*so)->Update();
	}

}
