#include "FrameBuffer.h"

namespace MocapGE
{
	FrameBuffer::FrameBuffer( void )
	{

	}

	FrameBuffer::~FrameBuffer( void )
	{

	}

	Camera* FrameBuffer::GetFrameCamera()
	{
		return viewport_->GetCamera();
	}


}
