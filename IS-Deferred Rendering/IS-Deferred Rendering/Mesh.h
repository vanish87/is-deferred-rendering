#ifndef MESH_H_
#define MESH_H_

#pragma once

namespace MocapGE
{
	class Mesh
	{
	public:
		Mesh(void);
		~Mesh(void);

	protected:

		RenderLayout* render_layout_;
	};

}

#endif
