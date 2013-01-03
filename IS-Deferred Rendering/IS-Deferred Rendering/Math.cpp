#include "Math.h"
#include "Vector.h"
#include "Ray.h"
#include "AABBox.h"

namespace MocapGE
{
	namespace Math
	{
		float Sqrt(float x)
		{
			return std::sqrt(x);
		}

		float Sin(float x)
		{
			return std::sin(x);
		}

		float Cos(float x)
		{
			return std::cos(x);
		}


		float Tan(float x)
		{
			return std::tan(x);
		}

		float Cot(float x)
		{
			return 1.0f / Tan(x);
		}
		float InvSqrt(float number)
		{
			float xhalf = 0.5f*number;

			int i = *(int*)&number; // get bits for floating value
			i = 0x5f3759df - (i>>1); // gives initial guess y0
			number = *(float*)&i; // convert bits back to float
			number = number*(1.5f-xhalf*number*number); // Newton step, repeating increases accuracy
			number = number*(1.5f-xhalf*number*number); // 2nd Newton step, repeating increases accuracy

			return number;
		}

		bool IntersectRayAABB( Ray* ray, AABBox* aabb )
		{
			//from http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
			http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-box-intersection/

			// r.dir is unit direction vector of ray
			float3 dirfrac, lb, rt;
			float t;
			lb = aabb->Min();
			rt = aabb->Max();
			float3 dir = ray->Direction();
			float3 org = ray->Origin();
			dirfrac.x() = 1.0f / dir.x();
			dirfrac.y() = 1.0f / dir.y();
			dirfrac.z() = 1.0f / dir.z();
			// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
			// r.org is origin of ray
			float t1 = (lb.x() - org.x())*dirfrac.x();
			float t2 = (rt.x() - org.x())*dirfrac.x();
			float t3 = (lb.y() - org.y())*dirfrac.y();
			float t4 = (rt.y() - org.y())*dirfrac.y();
			float t5 = (lb.z() - org.z())*dirfrac.z();
			float t6 = (rt.z() - org.z())*dirfrac.z();

			float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
			float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

			// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
			if (tmax < 0)
			{
				t = tmax;
				return false;
			}

			// if tmin > tmax, ray doesn't intersect AABB
			if (tmin > tmax)
			{
				t = tmax;
				return false;
			}

			t = tmin;
			return true;
		}

		bool IntersectRayTriangle( Ray* ray, float3 a, float3 b, float3 c )
		{
			//http://www.cs.washington.edu/education/courses/cse457/07sp/lectures/triangle_intersection.pdf
			float3 ab = b-a;
			float3 ac = c-a;
			float3 n = Math::Cross(ab, ac);
			if(Math::Dot(n, ray->Direction()) == 0) return false;
			float D = Math::Dot(n , a);
			float t = (D - Math::Dot(n, ray->Origin())) / Math::Dot(n, ray->Direction());

			if(t > 0)
			{
				float3 q = ray->Origin() + ray->Direction() * t;
				float3 bc = c - b;
				float3 ca = a - c;
				float3 aq = q - a;
				float3 bq = q - b;
				float3 cq = q - c;

				if(Math::Dot(Math::Cross(ab,aq), n) >= 0 && Math::Dot(Math::Cross(bc,bq), n) >= 0 && Math::Dot(Math::Cross(ca,cq), n) >= 0)
					return true;
				return false;
			}
			return false;
		}

	}
}