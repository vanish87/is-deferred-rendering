#ifndef PREDEC_H_
#define PREDEC_H_

//#pragma warning(disable:4819 4355) take care

//deal with the 'max' macro in windows.h colliding with 'max' in 'std'
#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#pragma once
#include <assert.h>
#include <boost/smart_ptr.hpp>

#define PRINT(x) std::cout<<x<<std::endl;

namespace MocapGE
{
	#define MOCAPGE_API __declspec(dllexport)
	#ifndef int32_t
	typedef __int32 int32_t;
	#endif
	typedef unsigned __int32 uint32_t;
	typedef __int64 int64_t;
	typedef unsigned __int64 uint64_t;

	class XMLParser;
	typedef boost::shared_ptr<XMLParser> XMLParserPtr;

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

	class App;
	class Window;
	class Timer;
	struct ContextConfig;
	struct RenderSetting;

	class Texture;
	class RenderState;
	class RenderFactory;
	class RenderEngine;
	class RenderView;
	class RenderLayout;
	class RenderElement;
	class RenderBuffer;
	class FrameBuffer;
	class Mesh;
	class Model;
	class DeferredRendering;

	class SceneObject;
	class SceneManager;

	class GameState;
	class StateManager;

	class Viewport;
	class Camera;
	class Frustum;


	template <typename T>
	class Sphere;
	typedef Sphere<float> shpere;
	class Ray;

	class AABBox;

	class Context;

	class PointLight;
	

}

#endif