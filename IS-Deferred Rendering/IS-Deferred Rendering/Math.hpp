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
		template <typename T>
		void Identity(Matrix<T> & lhs)
		{
				lhs =  Matrix<T>(1, 0, 0, 0,
								 0, 1, 0, 0,
								 0, 0, 1, 0,
								 0, 0, 0, 1 );
		}

		template <typename T>
		Matrix<T>  Inverse(Matrix<T> & lhs)
		{
			Matrix<T> dst;
			// COMPUTE ADJOINT COFACTOR MATRIX FOR THE ROTATION/SCALE 3x3 SUBMATRIX

			for (int i = 0 ; i < 3 ; i++)
				for (int j = 0 ; j < 3 ; j++) {
					int iu = (i + 1) % 3, iv = (i + 2) % 3;
					int ju = (j + 1) % 3, jv = (j + 2) % 3;
					dst[j][i] = lhs[iu][ju] * lhs[iv][jv] - lhs[iu][jv] * lhs[iv][ju];
				}

				// RENORMALIZE BY DETERMINANT TO INVERT ROTATION/SCALE SUBMATRIX

				double det = lhs[0][0]*dst[0][0] + lhs[1][0]*dst[0][1] + lhs[2][0]*dst[0][2];
				for (int i = 0 ; i < 3 ; i++)
					for (int j = 0 ; j < 3 ; j++)
						dst[i][j] /= det;

				// INVERT TRANSLATION

				for (int i = 0 ; i < 3 ; i++)
					dst[i][3] = -dst[i][0]*lhs[0][3] - dst[i][1]*lhs[1][3] - dst[i][2]*lhs[2][3];

				// NO PERSPECTIVE

				for (int i = 0 ; i < 4 ; i++)
					dst[3][i] = i < 3 ? 0 : 1;

				return dst;
		}

		template <typename T>
		Matrix<T>  Transpose(Matrix<T> & lhs)
		{
			return Matrix<T>(
				lhs(0, 0), lhs(1, 0), lhs(2, 0), lhs(3, 0),
				lhs(0, 1), lhs(1, 1), lhs(2, 1), lhs(3, 1),
				lhs(0, 2), lhs(1, 2), lhs(2, 2), lhs(3, 2),
				lhs(0, 3), lhs(1, 3), lhs(2, 3), lhs(3, 3));
		}

		template <typename T>
		Matrix<T>  InverTranspose(Matrix<T> & lhs)
		{
			return Transpose(Inverse(lhs));
		}

		template <typename T>
		void XRotation(Matrix<T> & lhs, float theta)
		{
			Identity(lhs);
			lhs[1][1] = Math::Cos(theta);
			lhs[1][2] = -Math::Sin(theta);
			lhs[2][1] = Math::Sin(theta);
			lhs[2][2] = Math::Cos(theta);
		}

		template <typename T>
		void YRotation(Matrix<T> & lhs, float theta)
		{
			Identity(lhs);
			lhs[0][0] = Math::Cos(theta);
			lhs[0][2] = Math::Sin(theta);
			lhs[2][0] = -Math::Sin(theta);
			lhs[2][2] = Math::Cos(theta);
		}

		template <typename T>
		void ZRotation(Matrix<T> & lhs, float theta)
		{
			Identity(lhs);
			lhs[0][0] = Math::Cos(theta);
			lhs[0][1] = -Math::Sin(theta);
			lhs[1][0] = Math::Sin(theta);
			lhs[1][1] = Math::Cos(theta);
		}

		template <typename T>
		void Translate(Matrix<T> & lhs, float x, float y, float z)
		{
			Identity(lhs);
			//left hand coordinate system
			lhs[3][0] = x;
			lhs[3][1] = y;
			lhs[3][2] = z;
		}

		template <typename T>
		void Scale(Matrix<T> & lhs, float scale)
		{
			Identity(lhs);

			lhs[0][0] = scale;
			lhs[1][1] = scale;
			lhs[2][2] = scale;
		}
	}
}

#endif