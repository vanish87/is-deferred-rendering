#include "Math.h"

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
		Vec3<T> Transform(const Vec3<T> & lhs, const Matrix<T> & rhs)
		{

		}

		template <typename T>
		Vec4<T>	Transform( const Vec4<T> & lhs, const Matrix<T> & rhs )
		{

		}

	}
}