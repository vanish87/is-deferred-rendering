#ifndef PREDEC_H_
#define PREDEC_H_

#pragma once
#include <assert.h>
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

	template <typename T>
	class Vec2;
	template <typename T>
	class Vec3;
	template <typename T>
	class Vec4;
	typedef Vec2<int32_t> int2;
	typedef Vec3<int32_t> int3;
	typedef Vec4<int32_t> int4;

	typedef Vec2<float> float2;
	typedef Vec3<float> float3;
	typedef Vec4<float> float4;

	template <typename T>
	class Matrix;
	typedef Matrix<float> float4x4;

	class Context;
	

}

#endif