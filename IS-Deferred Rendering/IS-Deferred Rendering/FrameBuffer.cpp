#include "FrameBuffer.h"

namespace MocapGE
{
	FrameBuffer::FrameBuffer( void )
	{

	}

	FrameBuffer::FrameBuffer( uint32_t left, uint32_t top, uint32_t width, uint32_t height)
		: left_(left), top_(top), width_(width), height_(height)
	{

	}

	FrameBuffer::~FrameBuffer( void )
	{

	}

	Camera* FrameBuffer::GetFrameCamera()
	{
		return viewport_->GetCamera();
	}

	void FrameBuffer::AddRenderView( RenderView* render_view )
	{
		//TODO : Check if the render_view is exist.
		render_views_.push_back(render_view);
	}

	void FrameBuffer::SetFrameCamera( Camera* camera )
	{
		viewport_->SetCamera(camera);
	}


}
