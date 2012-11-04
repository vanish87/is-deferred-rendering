#ifndef MATH_HPP_
#define MATH_HPP_

#pragma once


#include "PreDec.h"

namespace MocapGE
{
	namespace Math
	{

		template <typename T>
		Matrix<T>	Multiply( const Matrix<T> & lhs, const Matrix<T> & rhs )
		{
			assert( lhs.row() == rhs.col() );
			Matrix<T> ret;
			size_t dim = 4;

			for (int row = 0 ; row < dim ; row++)
				for (int col = 0 ; col < dim ; col++) 
				{
					ret[row][col] = 0;
					for (int i = 0 ; i < dim ; i++)
						ret[row][col] += lhs[row][i] * rhs[i][col];
				}
			return ret;
		}



		template <typename T>
		T Dot(Vec3<T> const & lhs, Vec3<T> const & rhs)
		{
			return T(lhs.x()*rhs.x() + lhs.y()*rhs.y() + lhs.z()* rhs.z());
		}

		template <typename T>
		Vec3<T> Cross(Vec3<T> const & lhs, Vec3<T> const & rhs)
		{
			return Vec3<T>(lhs.y() * rhs.z() - lhs.z() * rhs.y(),
				lhs.z() * rhs.x() - lhs.x() * rhs.z(),
				lhs.x() * rhs.y() - lhs.y() * rhs.x());
		}


		template <typename T>
		T Normalize(T const & rhs)
		{
			return rhs * InvSqrt(Dot(rhs , rhs));
		}
		template <typename T>
		Vec3<T> Transform(const Vec3<T> & lhs, const Matrix<T> & rhs)
		{

		}

		template <typename T>
		Vec4<T>	Transform( const Vec4<T> & lhs, const Matrix<T> & rhs )
		{

		}
		template <typename T>
		Matrix<T> LookAtLH( const Vec3<T> & eye, const Vec3<T> & at, const Vec3<T> & up )
		{
			Vec3<T> zaxis = Normalize(at - eye);
			Vec3<T> xaxis = Normalize(Cross(up, zaxis));
			Vec3<T> yaxis = Cross(zaxis, xaxis);

			return Matrix<T>(
				xaxis.x(),           yaxis.x(),           zaxis.x(),           0,
				xaxis.y(),           yaxis.y(),           zaxis.y(),           0,
				xaxis.z(),           yaxis.z(),           zaxis.z(),           0,
				-Dot(xaxis, eye),  -Dot(yaxis, eye),  -Dot(zaxis, eye),		   1);
		}
		template <typename T>
		Matrix<T> PerspectiveFovLH(const T fovy, const T aspect, const T zn,const T zf)
		{
			float y_scale = Cot(fovy / 2);
			float x_scale = y_scale / aspect;
			float m33 = zf/(zf-zn);

			return Matrix<T>(x_scale, 0,	   0,		0,
							 0,		  y_scale, 0,		0,
							 0,		  0,	   m33,		1,
							 0,		  0,	   -zn*m33, 0 );
		}
	}
}

#endif