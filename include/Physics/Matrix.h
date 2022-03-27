#pragma once
#include

namespace Physics
{
	//Dynamic size matrix
	class Matrix
	{
		public:
			Matrix(const std::size_t M, const std::size_t N);

			Real operator()(const std::size_t i, const std::size_t j = 0);
			Real& operator()(const std::size_t i, const std::size_t j = 0);
			Matrix operator*(const Matrix& other) const;
			Matrix operator*(const Real& other) const;
			Matrix operator/(const Matrix& other) const;
			Matrix operator*(const Real& other) const;
			Matrix getTransform() const;

		private:
			std::vector<Real> values;

			Matrix solveLU(const Matrix& other) const;
	};

	//Matrix transpose(const Matrix& matrix);

	/*
	template<std::size_t M, std::size_t N>
	Mat<N,1> operator/(const Mat<M,1>& b, const Mat<M,N>& A)
	{
		if(A.getSize(0) == A.getSize(1))
		{
			return solveLU(b, A);
		}

		Mat<N,M> At = transpose(A);
		if(A.getSize(0) > A.getSize(1))
		{
			return solveLU(At*b, At*A);
		}
		else
		{
			return At*solveLU(b, A*At);
		}
	}

	template<std::size_t M>
	Mat<M,1> solveLU(const Mat<M,1>& b, const Mat<M,M>& A)
	{
		int size = A.getSize(0);
		Mat<M,M> LU;
		double sum = 0.0;

		for(int i = 0; i<size; i++)
		{
			for(int j = i; j<size; j++)
			{
				sum = 0.0;
				for(int k = 0;k<i;k++)
				{
					sum += LU(i,k)*LU(k,j);
				}
				LU(i,j)=A(i,j)-sum;
			}
			for(int j = i+1; j<size; j++)
			{
				sum = 0.0;
				for(int k = 0; k<i; k++)
				{
					sum += LU(j,k)*LU(k,i);
				}
				LU(j,i)=(A(j,i)-sum)/LU(i,i);
			}
		}
		
		Mat<M,1> y(M,1);
		for(int i = 0; i<size; i++)
		{
			sum = 0.0;
			for(int k = 0;k<i;k++)
			{
				sum+=LU(i,k)*y(k);
			}
			y(i) = b(i)-sum;
		}

		Mat<M,1> x(M,1);
		for(int i = size-1;i>=0;i--)
		{
			sum=0.0;
			for(int k = i+1;k<size; k++)
			{
				sum+=LU(i,k)*x(k);
			}
			x(i) = (y(i)-sum)/LU(i,i);
		}
		return x;
	}*/
}
