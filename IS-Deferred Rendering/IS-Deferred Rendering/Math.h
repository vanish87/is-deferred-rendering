//
//Eggache implementing of template methods.
//The implementation of template class is in Math.hpp file.
//The rest of method implementation is in Math.cpp file.

#ifndef MATH_H_
#define MATH_H_

#pragma once

#include "PreDec.h"

namespace MocapGE
{
	namespace Math
	{
		template <typename T>
		Matrix<T> Multiply(const Matrix<T> & lhs, const Matrix<T> & rhs);

		float Sqrt(float x);
		float Tan(float x);
		float Cot(float x);
		float InvSqrt(float number);

		template <typename T> 
		T Dot(Vec3<T> const & lhs, Vec3<T> const & rhs);

		template <typename T> 
		Vec3<T> Cross(Vec3<T> const & lhs, Vec3<T> const & rhs);

		template <typename T> 
		T Normalize(T const & rhs);

		template <typename T> 
		Vec3<T> Transform(const Vec3<T> & lhs, const Matrix<T> & rhs);


		template <typename T>
		Vec4<T> Transform(const Vec4<T> & lhs, const Matrix<T> & rhs);

		template <typename T>
		Matrix<T> LookAtLH(const Vec3<T> & eye, const Vec3<T> & at, const Vec3<T> & up);

		template <typename T>
		Matrix<T> PerspectiveFovLH(const T fovy, const T aspect, const T zn,const T zf);

	}

}

#include "Math.hpp"
#endif