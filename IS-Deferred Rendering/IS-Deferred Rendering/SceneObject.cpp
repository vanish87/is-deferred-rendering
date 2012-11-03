#include "SceneObject.h"

namespace MocapGE
{
	SceneObject::SceneObject(void)
	{
	}


	SceneObject::~SceneObject(void)
	{
	}

	void SceneObject::Update()
	{

	}

	RenderElement* & SceneObject::GetRenderElement()
	{
		return render_element_;
	}

}
