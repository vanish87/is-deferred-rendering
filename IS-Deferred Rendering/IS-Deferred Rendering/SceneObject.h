#ifndef SCENEOBJECT_H_
#define SCENEOBJECT_H_

#pragma once
#include "PreDec.h"

namespace MocapGE
{
	class SceneObject
	{
	public:
		SceneObject(void);
		~SceneObject(void);

		virtual void Update() = 0;

	protected:

		RenderElement* render_element_;
	};

}

#endif

