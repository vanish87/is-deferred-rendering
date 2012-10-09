#ifndef PREDEC_H_
#define PREDEC_H_

#pragma once
#include <boost/smart_ptr.hpp>

namespace MocapGE
{
	#define MOCAPGE_API __declspec(dllexport)

	class XMLParser;
	typedef boost::shared_ptr<XMLParser> XMLParserPtr;

	class App;
	class Window;
	struct ContextConfig;
	struct RenderSetting;
	class RenderFactory;
	class RenderEngine;
	class RenderView;
	class FrameBuffer;

	class Viewport;
	class Camera;
	class Frustum;
	

}

#endif