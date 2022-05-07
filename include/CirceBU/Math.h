#pragma once
#include <assert.h>
#include <iostream>
#include <ostream>
#include <math.h>
#include <memory>
#include <vector>
#include "Profile.h"

namespace Circe
{	
	typedef double Real;

	//Fwd declaration
	struct Complex;
	struct Quaternion;
	struct Mat;
	template<std::size_t N> struct Vec;
	
	//A few useful functions
	template<typename Number>
	extern Number min(const Number& a, const Number& b)
	{
		return std::min(a,b);
	}
	
	template<typename Number>
	extern Number max(const Number& a, const Number& b)
	{
		return std::max(a,b);
	}
	
	template<std::size_t M>
	Real max(const Vec<M>& a)
	{
		Real maximum = a(0);
		for(int i = 1; i<M; i++)
		{
			maximum = std::max(maximum, a(i));
		}
		return maximum;
	}
	
	template<std::size_t M>
	Real min(const Vec<M>& a)
	{
		Real minimum = a(0);
		for(int i = 1; i<M; i++)
		{
			minimum = std::min(minimum, a(i));
		}
		return minimum;
	}
	
	
	//Scalar product
	template<std::size_t N>
	Real dot(const Vec<N>& a, const Vec<N>& b)
	{
		return a.dot(b);
	}
	
	//Inequality operator
	template<std::size_t N>
	bool operator!=(const Vec<N>& a, const Vec<N>& b)
	{
		return !(a==b);
	}
	
	//Add two vectors
	template<std::size_t N>
	Vec<N> operator+(const Vec<N>& a, const Vec<N>& b)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)+b(i);
		}
		return res;
	}
				
	//Subtract two vectors
	template<std::size_t N>
	Vec<N> operator-(const Vec<N>& a, const Vec<N>& b)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)-b(i);
		}
		return res;
	}
	
	//Negative vector
	template<std::size_t N>
	Vec<N> operator-(const Vec<N>& a)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=-a(i);
		}
		return res;
	}
	
	//Multiply vector term by term
	template<std::size_t N>
	Vec<N> operator*(const Vec<N>& a, const Vec<N>& b)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)*b(i);
		}
		return res;
	}
	
	//Divide vector term by term
	template<std::size_t N>
	Vec<N> operator/(const Vec<N>& a, const Vec<N>& b)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)/b(i);
		}
		return res;
	}
	
	//Add float to a vector
	template<std::size_t N>
	Vec<N> operator+(const Vec<N>& a, const Real& b)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)+b;
		}
		return res;
	}
	
	template<std::size_t N>
	Vec<N> operator+(const Real& b, const Vec<N>& a)
	{
		return a+b;
	}
	
	//Multiply float to this vector
	template<std::size_t N>
	Vec<N> operator*(const Vec<N>& a, const Real& b)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)*b;
		}
		return res;
	}
	
	template<std::size_t N>
	Vec<N> operator*(const Real& b, const Vec<N>& a)
	{
		return a*b;
	}
	
	//Divide float to this vector
	template<std::size_t N>
	Vec<N> operator/(const Vec<N>& a, const Real& b)
	{
		Vec<N> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)/b;
		}
		return res;
	}
	
	//Get the norm of the vector
	template<std::size_t N>
	Real norm(const Vec<N> a)
	{
		return sqrt(dot(a, a));
	}
	
	//Normalize the vector
	template<std::size_t N>
	Vec<N> normalize(Vec<N> v)
	{
		v=v/norm(v);
		return v;
	}
	
	//Get distance square
	template<std::size_t N>
	Real distanceSquare(const Vec<N>& a, const Vec<N>& b)
	{
		return (a-b).dot(a-b);
	}
	
	//Get distance
	template<std::size_t N>
	Real distance(const Vec<N>& a, const Vec<N>& b)
	{
		return sqrt(distanceSquare(a, b));
	}
	
	//Matrix multiplication, returns v*A
	/*template<std::size_t N, std::size_t M>
	Vec<M> operator*(const Vec<N>& a, const Mat<N, M>& m)
	{
		Vec<M> result;
		for(int j = 0; j<M; ++j)
		{
			Real value = (Real)0.0;
			for(int i=0; i<N;++i)
			{
				value+=a(i)*m(i,j);
			}
			result(j)=value;
		}
		return result;
	}*/

		

	template<std::size_t N>
	struct Vec
	{
		public:
			Vec()
			{
				for(int i=0;i<N;++i)
				{
					data[i]=(Real)0.0;
				}
			}
			
			Vec(const std::initializer_list<Real>& values)
			{
				size_t i = 0;
				for(Real f : values)
				{
					data[i]=f;
					i++;
				}
			}
			
			Vec(const std::vector<Real>& values)
			{
				size_t i = 0;
				for(Real f : values)
				{
					data[i]=f;
					i++;
				}
			}

			//Copy constructor
			Vec(const Vec<N>& v)
			{
				for(int i=0;i<N;++i)
				{
					data[i]=v(i);
				}
			}
			
			Vec(const Real& f)
			{
				for(int i=0;i<N;++i)
				{
					data[i]=f;
				}
			}
			
			//Copy assignment
			Vec<N>& operator=(const Vec<N>& b)
			{
				for(int i=0;i<N;++i)
				{
					data[i]=b(i);
				}
				return *this;
			}
			
			//Add a vector to this one
			void operator+=(const Vec<N>& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]+=b(i);
				}
			}
			
			//Subtract a vector to this one
			void operator-=(const Vec<N>& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]-=b(i);
				}
			}

			//Multiply vector term by term
			void operator*=(const Vec<N>& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]*=b(i);
				}
			}
			
			//Divide vector term by term
			void operator/=(const Vec<N>& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]/=b(i);
				}
			}

			//Add float to this vector
			void operator+=(const Real& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]+=b;
				}
			}
			
			//Subtract float to this vector
			void operator-=(const Real& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]-=b;
				}
			}

			//Multiply float to this vector
			void operator*=(const Real& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]*=b;
				}
			}
			
			//Divide float to this vector
			void operator/=(const Real& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]/=b;
				}
			}
			
			//Equality operator
			bool operator==(const Vec<N>& b) const
			{
				for(int i=0;i<N;++i)
				{
					if(data[i]!=b(i))
					{
						return false;
					}
				}
				return true;
			}
	
			//Dot product
			Real dot(const Vec<N>& b) const
			{
				Real result(0.0);
				for(int i=0; i<N;++i)
				{
					result+=data[i]*b(i);
				}
				return result;
			}
		
			//Get one element of this vector
			Real operator()(const unsigned int& index) const
			{
				assert(index<N);
				return data[index];
			}
	
			//Set one element of this vector
			Real &operator()(const unsigned int& index)
			{
				assert(index<N);
				return data[index];
			}
			
			void reset()
			{
				for(int i =0; i<N; ++i)
				{
					data[i]=0.0;
				}
			}
			
			std::size_t getSize() const
			{
				return N;
			}
			
		private:
			Real data[N];
	};


	template<>	
	struct Vec<2>
	{
		public:
			Vec():x((Real)0.0), y((Real)0.0)
			{}
		
			Vec(const Real& x, const Real& y):x(x), y(y)
			{}
	
			Vec(const std::vector<Real>& v):x(v[0]), y(v[1])
			{}		

			//Copy constructor
			Vec(const Vec<2>& v):x(v.x), y(v.y)
			{}
			
			Vec(const Real& f)
			{
				x=f;
				y=f;
			}
			
			//Copy assignment
			Vec<2>& operator=(const Vec<2>& b)
			{
				x=b.x;
				y=b.y;
				return *this;
			}
			
			//Add a vector to this one
			void operator+=(const Vec<2>& b)
			{
				x+=b.x;
				y+=b.y;
			}
			
			//Subtract a vector to this one
			void operator-=(const Vec<2>& b)
			{
				x-=b.x;
				y-=b.y;
			}

			//Multiply vector term by term
			void operator*=(const Vec<2>& b)
			{
				x*=b.x;
				y*=b.y;
			}
			
			//Divide vector term by term
			void operator/=(const Vec<2>& b)
			{
				x/=b.x;
				y/=b.y;
			}

			//Add float to this vector
			void operator+=(const Real& b)
			{
				x+=b;
				y+=b;
			}
			
			//Subtract float to this vector
			void operator-=(const Real& b)
			{
				x-=b;
				y-=b;
			}

			//Multiply float to this vector
			void operator*=(const Real& b)
			{
				x*=b;
				y*=b;
			}
			
			//Divide float to this vector
			void operator/=(const Real& b)
			{
				x/=b;
				y/=b;
			}
			
			//Equality operator
			bool operator==(const Vec<2>& b) const
			{
				return (x==b.x && y==b.y);
			}
	
			//Dot product
			Real dot(const Vec<2>& b) const
			{
				return x*b.x + y*b.y;
			}
	
			//Get one element of this vector
			inline Real operator()(const unsigned int& index) const
			{
				assert(index<2);
				if(index==0)return x;
				if(index==1)return y;
				return 0.0f;
			}
	
			//Set one element of this vector
			inline Real& operator()(const unsigned int& index)
			{
				assert(index<2);
				if(index==0)return x;
				if(index==1)return y;
				return x;
			}
			
			void reset()
			{
				x=(Real)0.0;
				y=(Real)0.0;
			}
			
			std::size_t getSize() const
			{
				return 2;
			}
			
		private:
			Real x, y;
	};

	template<>	
	struct Vec<3>
	{
		public:
			Vec():x((Real)0.0), y((Real)0.0f), z((Real)0.0f)
			{}
		
			Vec(const Real& x, const Real& y, const Real& z)
				:x(x), y(y), z(z)
			{}
	
			Vec(const std::vector<Real>& v):x(v[0]), y(v[1]), z(v[2])
			{}				

			//Copy constructor
			Vec(const Vec<3>& v):x(v.x), y(v.y), z(v.z)
			{}
			
			Vec(const Real& f)
			{
				x=f;
				y=f;
				z=f;
			}
			
			//Copy assignment
			Vec<3>& operator=(const Vec<3>& b)
			{
				x=b.x;
				y=b.y;
				z=b.z;
				return *this;
			}
			
			//Add a vector to this one
			void operator+=(const Vec<3>& b)
			{
				x+=b.x;
				y+=b.y;
				z+=b.z;
			}
			
			//Subtract a vector to this one
			void operator-=(const Vec<3>& b)
			{
				x-=b.x;
				y-=b.y;
				z-=b.z;
			}

			//Multiply vector term by term
			void operator*=(const Vec<3>& b)
			{
				x*=b.x;
				y*=b.y;
				z*=b.z;
			}
			
			//Divide vector term by term
			void operator/=(const Vec<3>& b)
			{
				x/=b.x;
				y/=b.y;
				z/=b.z;
			}

			//Add float to this vector
			void operator+=(const Real& b)
			{
				x+=b;
				y+=b;
				z+=b;
			}
			
			//Subtract float to this vector
			void operator-=(const Real& b)
			{
				x-=b;
				y-=b;
				z-=b;
			}

			//Multiply float to this vector
			void operator*=(const Real& b)
			{
				x*=b;
				y*=b;
				z*=b;
			}
			
			//Divide float to this vector
			void operator/=(const Real& b)
			{
				x/=b;
				y/=b;
				z/=b;
			}
			
			//Equality operator
			bool operator==(const Vec<3>& b) const
			{
				return (x==b.x && y==b.y && z==b.z);
			}
	
			//Dot product
			Real dot(const Vec<3>& b) const
			{
				return x*b.x + y*b.y + z*b.z;
			}
			
			//Get one element of this vector
			inline Real operator()(const unsigned int& index) const
			{
				assert(index<3);
				if(index==0)return x;
				if(index==1)return y;
				if(index==2)return z;
				return (Real)0.0;
			}
	
			//Set one element of this vector
			inline Real& operator()(const unsigned int& index)
			{
				assert(index<3);
				if(index==0)return x;
				if(index==1)return y;
				if(index==2)return z;
				return x;
			}
			
			void reset()
			{
				x=(Real)0.0;
				y=(Real)0.0;
				z=(Real)0.0;
			}
			
			std::size_t getSize() const
			{
				return 3;
			}
	
			
		private:
			Real x, y, z, padding;
	};
	
	using Vec2=Vec<2>;
	using Vec3=Vec<3>;

	struct Mat
	{	
		public:
			Mat()
			{}

			Mat(const std::size_t newM, const std::size_t newN)
			{	
				init(newM, newN);	
			}
		
			void init(const std::size_t newM, const std::size_t newN)
			{
				M = newM;
				N = newN;
				data.clear();
				for(int k = 0; k<M*N; k++)
					data.push_back(0.0);
			}

			Mat(const std::size_t M, const std::size_t N,
				const std::initializer_list<Real>& values)
			{
				init(M,N);
				std::size_t k = 0;
				for(Real f : values)
				{
					data[k] = f;
					k++;
				}

			}

			Mat(const std::initializer_list<Mat>& values)
			{
				for(Mat m : values)
				{
					M  = max(M,m.M);
					N += m.N;
				}

				init(M,N);

				int j0 = 0;
				for(Mat m : values)
				{
					for(int j = 0; j<m.N; j++)
					{
						for(int i = 0; i<m.M; i++)
						{
							(*this)(i,j0) = m(i,j);
						}
						j0++;
					}
				}
			}
			
			
			Mat& operator=(const Mat& m)
			{
				init(m.M, m.N);
				data = m.data;	
				return *this;
			}
			
		
			//Matrix multiplication to matrix
			Mat operator*(const Mat& m) const
			{
				assert(N == m.M);

				Mat result(M,m.N);
				for(int i = 0; i<M; ++i)
				{
					for(int j = 0; j<m.N; ++j)
					{
						Real value = (Real)0.0;
						for(int k=0;k<N; ++k)
						{
							value+=(*this)(i,k)*m(k,j);
						}
						result(i,j)=value;
					}
				}	
				return result;
			}
				
			Mat transposeMult(const Mat& m) const
			{
				assert(M == m.M);

				Mat result(N,m.N);
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<m.N; ++j)
					{
						Real value = (Real)0.0;
						for(int k=0;k<M; ++k)
						{
							value+=(*this)(k,i)*m(k,j);
						}
						result(i,j)=value;
					}
				}	
				return result;
			}
			
			Mat multTranspose(const Mat& m) const
			{
				assert(N == m.N);

				Mat result(M,m.M);
				for(int i = 0; i<M; ++i)
				{
					for(int j = 0; j<m.M; ++j)
					{
						Real value = (Real)0.0;
						for(int k=0;k<N; ++k)
						{
							value+=(*this)(i,k)*m(j,k);
						}
						result(i,j)=value;
					}
				}	
				return result;
			}	

			//Get the matrix multiplied by a scalar
			Mat operator*(const Real& f) const
			{
				Mat result(M,N);
				for(int k = 0; k<M*N; k++)
				{
					result.data[k] = data[k]*f;
				}	
				return result;	
			}

			Mat operator/(const Real& f) const
			{
				Mat result(M,N);
				for(int k = 0; k<M*N; k++)
				{
					result.data[k] = data[k]/f;
				}	
				return result;	
			}
			
			void operator+=(const Mat& m)
			{
				assert(M == m.M && N == m.N);
				for(int k = 0; k<M*N; k++)
				{
					data[k]+=m.data[k];
				}	
			}
			
			Mat operator+(const Mat& m) const
			{
				assert(M == m.M && N == m.N);
				Mat result(M,N);
				for(int k = 0; k<M*N; k++)
				{
					result.data[k] = data[k] + m.data[k];
				}	
				return result;	
			}
				
			Mat operator-(const Mat& m) const
			{
				assert(M == m.M && N == m.N);
				Mat result(M,N);
				for(int k = 0; k<M*N; k++)
				{
					result.data[k] = data[k] - m.data[k];
				}	
				return result;	
			}

			void operator-=(const Mat& m)
			{
				assert(M == m.M && N == m.N);
				for(int k = 0; k<M*N; k++)
				{
					data[k]-=m.data[k];
				}	
			}
			
			//Multiply by a scalar
			void operator*=(const Real& f)
			{
				for(int k = 0; k<M*N; k++)
				{
					data[k]*=f;
				}
			}

			/** Solve the LU factorization of Ax = b with b = this */
			Mat operator/(const Mat& A)
			{
				assert(A.M == M);

				if(A.M == A.N)
				{
					return solveLU(A);
				}

				if(A.M > A.N)
				{
					return (A.transposeMult(*this))*
								solveLU(A.transposeMult(A));
				}
				else
				{
					return A.transposeMult(*this)*
								solveLU(A.multTranspose(A));
				}
			}

			Mat solveLU(const Mat& A)
			{
				assert(M == A.M && M == A.N);

				Mat LU(M,M);
				double sum = 0.0;

				for(int i = 0; i<M; i++)
				{
					for(int j = i; j<M; j++)
					{
						sum = 0.0;
						for(int k = 0;k<i;k++)
						{
							sum += LU(i,k)*LU(k,j);
						}
						LU(i,j)=A(i,j)-sum;
					}
					for(int j = i+1; j<M; j++)
					{
						sum = 0.0;
						for(int k = 0; k<i; k++)
						{
							sum += LU(j,k)*LU(k,i);
						}
						LU(j,i)=(A(j,i)-sum)/LU(i,i);
					}
				}
				
				Mat y(M,1);
				for(int i = 0; i<M; i++)
				{
					sum = 0.0;
					for(int k = 0;k<i;k++)
					{
						sum+=LU(i,k)*y(k);
					}
					y(i) = (*this)(i)-sum;
				}

				Mat x(M,1);
				for(int i = M-1;i>=0;i--)
				{
					sum=0.0;
					for(int k = i+1;k<M; k++)
					{
						sum+=LU(i,k)*x(k);
					}
					x(i) = (y(i)-sum)/LU(i,i);
				}
				return x;
			}
					
			//Get the coefficient at row i and column j
			Real operator()(const int& i, const int& j = 0) const
			{
				assert(i<M && j<N);
				return data[i*N+j];
			}
			
			//Set the coefficient at row i and column j
			Real& operator()(const int& i, const int& j = 0)
			{
				assert(i<M && j<N);
				return data[i*N+j];
			}

			std::size_t getNbRows() const
			{
				return M;
			}
			
			std::size_t getNbCols() const
			{
				return N;
			}
			
		private:
			std::vector<Real> data;
			std::size_t M,N;
	};
	
	//using Mat44=Mat<4, 4>;
	static Real cross(const Vec<2>& a, const Vec<2>& b)
	{
		return a(0)*b(1)-a(1)*b(0);
	}
	
	static Vec<3> cross(const Vec<3>& a, const Vec<3>& b)
	{
		return Vec<3>(a(1)*b(2)-a(2)*b(1),
					 -a(0)*b(2)+a(2)*b(0),
					  a(0)*b(1)-a(1)*b(0));
	}

	static Vec<2> tripleProduct(const Vec<2>& a, const Vec<2>& b,
								const Vec<2>& c)
	{
		Real d = cross(a,b);
		return Vec<2>(-c(1)*d, c(0)*d);
	}

	static Vec<3> tripleProduct(const Vec<3>& a, const Vec<3>& b,
								const Vec<3>& c)
	{
		return cross(cross(a,b),c);
	}

	struct Complex
	{
		public:
			inline Complex(const Real& angle):c(cos(angle)), s(sin(angle))
			{}
			
			inline Complex(const Real& real, const Real& imaginary):
							c(real), s(imaginary)
			{}
	
			inline	Complex(const Mat& m):Complex(m(0,0), m(0,1))
			{}

			inline Real getAngle() const
			{
				return atan2(s,c);
			}
			
			inline Complex operator*(const Complex& q) const
			{
				Real a=c;Real b=s;
				Real c=q.c;Real d=q.s;
				return Complex(a*c-b*d, b*c+a*d);
			}
			
			inline Complex operator=(const Complex& q)
			{
				c=q.c;
				s=q.s;
				return *this;
			}
			
			inline Real length() const
			{
				return sqrt(c*c+s*s);
			}
			
			inline Complex normalize()
			{
				Real length = (*this).length();
				c/=length;
				s/=length;
				return *this;
			}
			
			inline Complex conjugate()
			{
				s*=-(Real)(1.0);
				return *this;
			}
			
			inline Real getReal() const
			{
				return c;
			}
			
			inline Real getImaginary() const
			{
				return s;
			}
			
			inline void addAngle(const Real& dtheta)
			{
				Complex q(dtheta);
				Real ctemp = c*q.c - s*q.s;
				s = c*q.s + s*q.c;
				c=ctemp;
			}
	
		private:
			Real c,s;
	};
	
	
	class Quaternion
	{
		public:
			inline Quaternion():w((Real)1.0), x(0.0), y(0.0), z(0.0)
			{}
			
			inline Quaternion(const Real& w, const Real& x, 
							  const Real& y, const Real& z):
								w(w), x(x), y(y), z(z)
			{}
			
			inline Quaternion(const Real& roll, const Real& pitch, 
								const Real& yaw)
			{
				Real cy = cos(yaw*0.5);
				Real sy = sin(yaw*0.5);
				Real cp = cos(pitch*0.5);
				Real sp = sin(pitch*0.5);
				Real cr = cos(roll*0.5);
				Real sr = sin(roll*0.5);
				
				w = cy*cp*cr+sy*sp*sr;
				x = cy*cp*sr-sy*sp*cr;
				y = sy*cp*sr+cy*sp*cr;
				z = sy*cp*cr-cy*sp*sr;
				
				//Source: Wikipedia, conversion between 
				// quaternion and Euler angles
			}
	
			inline Quaternion(const Mat& m)
			{
				Real trace = m(0, 0) + m(1, 1) + m(2, 2);
				
				if(trace > 0)
				{
					Real s = 0.5 / (Real)std::sqrt(trace+ 1.0);
					w = 0.25 / s;
					x = (m(1, 2) - m(2, 1)) * s;
					y = (m(2, 0) - m(0, 2)) * s;
					z = (m(0, 1) - m(1, 0)) * s;
				}
				else
				{
					if(m(0, 0) > m(1, 1) && m(0, 0) 
							> m(2, 2))
					{
						Real s = 2.0f * (Real)std::sqrt(1.0f + m(0, 0)
							   		- m(1, 1) - m(2, 2));
						w = (m(1, 2) - m(2, 1)) / s;
						x = 0.25f * s;
						y = (m(1, 0) + m(0, 1)) / s;
						z = (m(2, 0) + m(0, 2)) / s;
					}
					else if(m(1, 1) > m(2, 2))
					{
						Real s = 2.0f * (Real)std::sqrt(1.0f + m(1, 1)
							   	- m(0, 0) - m(2, 2));
						w = (m(2, 0) - m(0, 2)) / s;
						x = (m(1, 0) + m(0, 1)) / s;
						y = 0.25f * s;
						z = (m(2, 1) + m(1, 2)) / s;
					}
					else
					{
						Real s = 2.0f * (Real)std::sqrt(1.0f + m(2, 2)
							   	- m(0, 0) - m(1, 1));
						w = (m(0, 1) - m(1, 0) ) / s;
						x = (m(2, 0) + m(0, 2) ) / s;
						y = (m(1, 2) + m(2, 1) ) / s;
						z = 0.25f * s;
					}
				}

				Real length = -(Real)std::sqrt(x*x + y*y + z*z + w*w);
				x /= length;
				y /= length;
				z /= length;
				w /= length;
				
			}

			inline Quaternion operator=(const Quaternion& q)
			{
				w=q.w;
				x=q.x;
				y=q.y;
				z=q.z;
				return *this;
			}
			
			inline Quaternion operator*(const Quaternion& q) const
			{
				return Quaternion(
					w*q.w - x*q.x - y*q.y - z*q.z, //w
					w*q.x + x*q.w + y*q.z - z*q.y, //x
					w*q.y - x*q.z + y*q.w + z*q.x, //y
					w*q.z + x*q.y - y*q.x + z*q.w  //z
				);
			}
			
			inline void operator*=(const Quaternion& q)
			{
				Real w0 = w*q.w - x*q.x - y*q.y - z*q.z;
				Real x0 = w*q.x + x*q.w + y*q.z - z*q.y;
				Real y0 = w*q.y - x*q.z + y*q.w + z*q.x;
				Real z0 = w*q.z + x*q.y - y*q.x + z*q.w;
				w = w0;
				x = x0;
				y = y0;
				z = z0;
			}
			
			inline void addAngle(const Real& roll, const Real& pitch, 
								 const Real& yaw)
			{
				
				Quaternion q = (*this)*Quaternion(0.0, 
												  -roll*0.5, 
												  -pitch*0.5, 
												  -yaw*0.5);
				w+=q.w;
				x+=q.x;
				y+=q.y;
				z+=q.z;
						
				normalize();
			}
			
			inline Real length() const
			{
				return sqrt(w*w+x*x+y*y+z*z);
			}
			
			inline Quaternion normalize()
			{
				Real length = (*this).length();
				w/=length;
				x/=length;
				y/=length;
				z/=length;

				return *this;
			}
			
			inline Quaternion conjugate()
			{
				x*=-1.0;
				y*=-1.0;
				z*=-1.0;

				return *this;
			}
			
			inline Quaternion getConjugate() const
			{
				return Quaternion(w, -x, -y, -z);
			}
			
			Vec<2> rotate(const Vec<2>& v2) const
			{
				Vec<3> r = rotate(Vec<3>(v2(0), v2(1), 0.0));
				return Vec<2>(r(0), r(1));
			}

			Vec<3> rotate(const Vec<3>& v2) const
			{
				if(v2.dot(v2) < 1e-10) return Vec<3>(v2(0),v2(1),v2(2));
				Quaternion v((Real)0.0, v2(0), v2(1), v2(2));
				Quaternion p(*this);
				p.normalize();
				Quaternion pConj=p.getConjugate();
				Quaternion result = p*v*pConj;
				Vec<3> v3;
				v3(0)=result.getX();
				v3(1)=result.getY();
				v3(2)=result.getZ();
				return v3;
			}
			
			Vec<2> rotateInv(const Vec<2>& v2) const
			{
				Vec<3> r = rotateInv(Vec<3>(v2(0), v2(1), 0.0));
				return Vec<2>(r(0), r(1));
			}

			Vec<3> rotateInv(const Vec<3>& v2) const
			{
				if(v2.dot(v2) < 1e-10) return Vec<3>(v2(0),v2(1),v2(2));
				Quaternion v((Real)0.0, v2(0), v2(1), v2(2));
				Quaternion p(*this);
				p.normalize();
				Quaternion pConj=p.getConjugate();
				Quaternion result = pConj*v*p;
				Vec<3> v3;
				v3(0)=result.getX();
				v3(1)=result.getY();
				v3(2)=result.getZ();
				return v3;
			}

			/** https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles  */
			Vec<3> toEulerAngle() const
			{
				Real sinr_cosp = 2.0*(w*x+y*z);
				Real cosr_cosp = 1.0-2.0*(x*x+y*y);
				Real roll = (Real)(std::atan2(sinr_cosp, cosr_cosp));

				Real sinp = 2.0*(w*y-z*x);
				Real pitch = std::asin(sinp);
				if(std::abs(sinp) >= 1)
					pitch = std::copysign(3.14159/2.0, sinp);

				Real siny_cosp = 2.0*(w*z+x*y);
				Real cosy_cosp = 1.0-2.0*(y*y+z*z);
				Real yaw = (Real)(std::atan2(siny_cosp, cosy_cosp));

				return Vec<3>(roll, pitch, yaw);
			}

			Real angleWith(const Quaternion& other)
			{
				Vec<3> v1 = rotate(Vec<3>(1,0,0)); 
				Vec<3> v2 = other.rotate(Vec<3>(1,0,0)); 
				Vec<3> v3 = cross(v1,v2);
			
				if(dot(v3,Vec<3>(0,0,1)) > 0)
					return -std::acos(dot(v1,v2));
				
				return std::acos(dot(v1,v2));
			}

			void lookAt(const Vec<2>& lookAt)
			{
				Vec<2> fwd = Circe::normalize(lookAt);

				Mat m(3,3);

				m(0,0) = fwd(0);
				m(1,0) = fwd(1);
				m(0,1) = -fwd(1);
				m(1,1) = fwd(0);
				m(2,2) = 1.0;

				Quaternion q(m);

				w = q.w;
				x = q.x;
				y = q.y;
				z = q.z;
			}

			void lookAt(const Vec<3>& lookAt, const Vec<3>& up0)
			{
				Vec<3> fwd = Circe::normalize(lookAt);
				Vec<3> right = Circe::normalize(cross(up0, fwd));
				Vec<3> up = cross(fwd, right);

				Mat m(3,3);
				for(int i = 0; i<3; i++)
				{
					m(i,0) = fwd(i);
					m(i,1) = right(i);
					m(i,2) = up(i);
				}

				Quaternion q(m);

				w = q.w;
				x = q.x;
				y = q.y;
				z = q.z;
			}

			inline Real getW() const
			{
				return w;
			}
			
			inline Real getX() const
			{
				return x;
			}
			
			inline Real getY() const
			{
				return y;
			}
			
			inline Real getZ() const
			{
				return z;
			}
	
		private:
			Real x,y,z,w;
	};
	


	static Mat transpose(const Mat& m)
	{
		Mat result(m.getNbCols(),m.getNbRows());

		for(int i = 0; i<m.getNbRows(); ++i)
		{
			for(int j = 0; j<m.getNbCols(); ++j)
			{
				result(j,i)=m(i,j);
			}
		}	

		return result;
	}

	static Mat identity(const std::size_t M, const std::size_t N)
	{
		Mat result(M,N);

		for(int i = 0; i<min(N, M); ++i)
		{
			result(i,i) = 1.0;
		}
		
		return result;	
	}

	static Mat crossMatrix(const Vec<3> a)
	{
		Mat m(3,3);

		m(0,1) = -a(2);
		m(0,2) =  a(1);
		m(1,0) =  a(2);
		m(1,2) = -a(0);
		m(2,0) = -a(1);
		m(2,1) =  a(0);

		return m;	
	}

	static Mat positionMatrix(const Real& x, const Real& y, 
								const Real& z)
	{
		Mat m = identity(4,4);
		m(0,3)=x;
		m(1,3)=y;
		m(2,3)=z;
		return m;
	}
	
	static Mat positionMatrix(const Vec2& v)
	{
		return positionMatrix(v(0), v(1), 0.0);
	}
	
	static Mat positionMatrix(const Vec3& v)
	{
		return positionMatrix(v(0), v(1), v(2));
	}
	
	static Mat scaleMatrix(const Real& x, const Real& y, 
							 const Real& z)
	{
		Mat m = identity(4,4);
		m(0,0)=x;
		m(1,1)=y;
		m(2,2)=z;

		return m;
	}

	static Mat scaleMatrix(const Vec<2>& v)
	{
		return scaleMatrix(v(0), v(1), 1.0f);
	}
	
	static Mat scaleMatrix(const Vec<3>& v)
	{
		return scaleMatrix(v(0), v(1), v(2));
	}
	
	//Get the rotation matrix
	static Mat rotationMatrix(const Complex& q)
	{
		Real c = q.getReal();
		Real s = q.getImaginary();
		
		Mat m(4,4);
		
		m(0,0)=c;		m(0,1)=s;		m(0,2)=0.0f;	m(0,3)=0.0f;
		m(1,0)=-s;		m(1,1)=c;		m(1,2)=0.0f;	m(1,3)=0.0f;
		m(2,0)=0.0f;	m(2,1)=0.0f;	m(2,2)=1.0f;	m(2,3)=0.0f;
		m(3,0)=0.0f;	m(3,1)=0.0f;	m(3,2)=0.0f;	m(3,3)=1.0f;
		
		return m;
	}
	
	static Mat rotationMatrix(const Quaternion& q)
	{
		Real x = q.getX();
		Real y = q.getY();
		Real z = q.getZ();
		Real w = q.getW();
		
		Mat m(4,4);
		
		m(0,0)=1.0f-(2.0f*y*y+2.0f*z*z);	
		m(0,1)=2.0f*x*y+2.0f*z*w;			
		m(0,2)=2.0f*x*z-2.0f*y*w;
		m(0,3)=0.0f;

		m(1,0)=2.0f*x*y-2.0f*z*w;	
		m(1,1)=1.0f-(2.0f*x*x+2.0f*z*z);
		m(1,2)=2.0f*y*z+2.0f*x*w;
		m(1,3)=0.0f;

		m(2,0)=2.0f*x*z+2.0f*y*w;		
		m(2,1)=2.0f*y*z-2.0f*x*w;			
		m(2,2)=1.0f-(2.0f*x*x+2.0f*y*y);	
		m(2,3)=0.0f;

		m(3,0)=0.0f;
		m(3,1)=0.0f;			
		m(3,2)=0.0f;
		m(3,3)=1.0f;
		
		return m;
	}
	
	static Mat rotationMatrix(const Vec<2>& fwdAxis)
	{
		Vec<2> f = fwdAxis;
		normalize(f);
		
		Mat rotationMatrix(4,4);

		rotationMatrix(0,0) = f(0);
		rotationMatrix(1,0) = f(1);
		rotationMatrix(0,1) = -f(0);
		rotationMatrix(1,1) = f(1);
		
		rotationMatrix(2,2) = 1.0f;
		rotationMatrix(3,3) = 1.0f;
		return rotationMatrix;
	}
	
	static Mat rotationMatrix(const Vec<3>& leftAxis, 
							  const Vec<3>& fwdAxis)
	{
		Vec<3> l = leftAxis;
		normalize(l);
		Vec<3> f = fwdAxis;
		if(l.dot(fwdAxis) != 0)
		{
			f = f - (f.dot(l))*l;
		}
		normalize(f);
		Vec<3> u = cross(l, f);
		
		Mat rotationMatrix(4,4);
		for(int i = 0; i<3; i++)
		{
			rotationMatrix(i,0) = l(i);
			rotationMatrix(i,1) = f(i);
			rotationMatrix(i,2) = u(i);
		}
		rotationMatrix(3,3) = 1.0f;
		return rotationMatrix;
	}

	static Mat orthoProjection(const Real& width, const Real& height, 
							const Real& nearField, const Real& farField)
	{
		Real left=-width/2.0;
		Real right=width/2.0;
		Real bottom=-height/2.0;
		Real top=height/2.0;
		Mat m(4,4);
		m(0,0)=2.0/width;
		m(1,1)=2.0/height;
		m(2,2)=2.0/(farField-nearField);
		m(3,3)=1.0;
		
		m(0,3)=-(right+left)/(right-left);
		m(1,3)=-(top+bottom)/(top-bottom);
		m(2,3)=-(farField+nearField)/(farField-nearField);
		
		return m;
	}
	
	static Mat perspectiveProjection(const Real& fieldOfView, 
									   const Real& aspectRatio, 
									   const Real& nearField, 
									   const Real& farField)
	{
		Mat m(4,4);
		Real tanHalfFov=std::tan(fieldOfView*3.141593/(2.0*180.0));
		Real range = farField-nearField;
		m(0,0) = 1.0/(aspectRatio*tanHalfFov);
		m(1,1) = 1.0/(tanHalfFov);
		m(2,2) = (nearField+farField)/range;
		m(2,3) = -2.0*farField*nearField/range;
		m(3,2) = 1.0;
		return m;
	}

	template<std::size_t N>
	class Trans
	{
		public:
			inline Trans(const Vec<N>& position0 = Vec<N>(),
					   const Quaternion& rotation = Quaternion(), 
					   const Vec<N>& scale = Vec<N>(1.0)) :
						position(position0), rotation(rotation),
						scale(scale)
			{}

			
			inline Trans(const Trans<N>& transform)
			{
				position = transform.position;
				rotation = transform.rotation;
				scale = transform.scale;
			}
			
			inline Trans<N>& operator=(const Trans<N>& transform)
			{	
				position = transform.position;
				rotation = transform.rotation;
				scale = transform.scale;
				return *this;
			}

			inline Vec<N>& getPosition()
			{
				return position;
			}
			
			inline Vec<N> getPosition() const
			{
				return position;
			}

			inline void setPosition(const Vec<N>& newPosition)
			{				
				position = newPosition;			
			}
			
			inline Quaternion& getRotation()
			{
				return rotation;
			}

			inline Quaternion getRotation() const
			{
				return rotation;
			}		

			inline void setRotation(const Quaternion& other)
			{
				rotation = other;
			}

			inline Vec<N>& getScale()
			{
				return scale;
			}
			
			inline Vec<N> getScale() const
			{
				return scale;
			}

			inline void setScale(const Vec<N>& newScale)
			{
				scale = newScale;
			}
			
			inline void rotate(const Real angle)
			{
				rotation.addAngle(0.0, 0.0, angle);
			}

			inline void rotate(const Vec3& eulerAngles)
			{
				rotation.addAngle(eulerAngles(0), eulerAngles(1), 
								eulerAngles(2));
			}
			
			inline void translate(const Vec<N>& v)
			{
				position += v;
			}
			
			inline void resize(const Real& scaleRatio)
			{
				scale*=scaleRatio;
			}
			
			inline Mat getTransformMatrix() const
			{
				Mat res = (positionMatrix(position)) 
						   * (rotationMatrix(rotation)) 
						   * (scaleMatrix(scale));

				return res;
			}

			inline Vec<N> toGlobal(const Vec<N>& vec, const bool translate)
			{
				Vec<N> res = rotation.rotateInv(vec);

				if(translate)
					res = res + position;

				return res;
			}

			inline Vec<N> toLocal(const Vec<N>& vec, const bool translate)
			{
				Vec<N> res = vec;

				if(translate)
					res = res - position;

				res = rotation.rotate(res);

				return res;
			}

			inline Real angleWith(const Trans<N>& other)
			{
				return rotation.angleWith(other.rotation);
			}

			inline void lookAt(const Vec<2>& location)
			{
				rotation.lookAt(location-position);
			}
			
			inline void lookAt(const Vec<3>& location)
			{
				rotation.lookAt(location-position, Vec<3>(0,0,1));
			}

		
			/** Modify the transform such that one end is a location p1 
			  	and the other end is at location p2 (useful for lines)
			 */	
			inline void lineUp(const Vec<N>& p1, const Vec<N>& p2, 
							   const Real scaleFactor = 1.0)
			{	
				position = (p1+p2)/2.0;

				scale(0) = norm(p1-p2)*scaleFactor;	

				lookAt(p2);
			}	

		private:
			Vec<N> position;
			Quaternion rotation;
			Vec<N> scale;
	};
	

	inline std::ostream& operator<<(std::ostream &strm, const Complex &q)
	{
		strm << "[" << q.getReal() << " + i" << q.getImaginary();
		return strm << "]";
	}
	
	inline std::ostream& operator<<(std::ostream &strm, 
									const Quaternion &q)
	{
		strm << "[" << q.getW() << ", " << q.getX() 
			 << ", " << q.getY() << ", " << q.getZ();
		return strm << "]";
	}
	
	template<std::size_t N>
	std::ostream& operator<<(std::ostream &strm, const Vec<N> &v)
	{
		strm << "[" << v(0);
		for(int i=1; i<N;++i){
			strm << ", " << v(i);
		}
		return strm << "]";
	}

	inline std::ostream& operator<<(std::ostream &strm, const Mat& m)
	{
		std::cout.precision(3);
		strm << m.getNbRows() << " x " << m.getNbCols() << std::endl;
		for(int i=0; i<m.getNbRows(); i++){
			strm << "|" << m(i,0);
			for(int j=1; j<m.getNbCols(); j++)
			{
				strm << ",\t" << m(i,j);
			}
			strm << "|" << std::endl;
		}
		return strm;
	}
	
}
