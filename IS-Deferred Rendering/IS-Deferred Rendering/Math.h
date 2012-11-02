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

		template <typename T>
		Vec3<T> Transform(const Vec3<T> & lhs, const Matrix<T> & rhs);


		template <typename T>
		Vec4<T> Transform(const Vec4<T> & lhs, const Matrix<T> & rhs);
	}



}

#endif