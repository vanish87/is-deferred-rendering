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
		 this->model_matrix_ = render_element_->model_matrix_;
	}

	RenderElement* & SceneObject::GetRenderElement()
	{
		return render_element_;
	}

}
