#pragma once
#include <assert.h>
#include <iostream>
#include <ostream>
#include <math.h>
#include <memory>
#include <vector>

namespace Circe
{	
	//Fwd declaration
	struct Complex;
	struct Quaternion;
	template<std::size_t N, typename Real> struct Vec;
	template<typename Real> struct Vec<2,Real>;
	template<typename Real> struct Vec<3,Real>;
	template<typename Real> struct Vec<4,Real>;
	template<std::size_t N, std::size_t M, typename Real> struct Mat;
	struct Transform3;
	
	//A few useful functions
	template<typename Real>
	extern Real min(const Real& a, const Real& b)
	{
		return std::min(a,b);
	}
	
	template<typename Real>
	extern Real max(const Real& a, const Real& b)
	{
		return std::max(a,b);
	}
	
	template<std::size_t M, typename Real>
	Real max(const Vec<M,Real>& a)
	{
		Real maximum = a(0);
		for(int i = 1; i<M; i++)
		{
			maximum = std::max(maximum, a(i));
		}
		return maximum;
	}
	
	template<std::size_t M, typename Real>
	Real min(const Vec<M,Real>& a)
	{
		Real minimum = a(0);
		for(int i = 1; i<M; i++)
		{
			minimum = std::min(minimum, a(i));
		}
		return minimum;
	}
	
	/*template<typename Real>
	extern Real operator ""_deg(const Real& angle)
	{
		return angle*3.141596/180.0;
	}
	
	template<typename Real>
	extern Real operator ""_rad(long double angle)
	{
		return angle;
	}*/
	
	template<typename Real>
	Real cross(const Vec<2, Real>& a, const Vec<2, Real>& b)
	{
		return a(0)*b(1)-a(1)*b(0);
	}
	
	template<typename Real>
	Vec<3, Real> cross(const Vec<3, Real>& a, const Vec<3, Real>& b)
	{
		return Vec<3, Real>(a(1)*b(2)-a(2)*b(1),-a(0)*b(2)+a(2)*b(0),a(0)*b(1)-a(1)*b(0));
	}
	
	//Scalar product
	template<std::size_t N, typename Real>
	Real dot(const Vec<N, Real>& a, const Vec<N, Real>& b)
	{
		return a.dot(b);
	}
	
	//Inequality operator
	template<std::size_t N, typename Real>
	bool operator!=(const Vec<N, Real>& a, const Vec<N, Real>& b)
	{
		return !(a==b);
	}
	
	//Add two vectors
	template<std::size_t N, typename Real>
	Vec<N, Real> operator+(const Vec<N, Real>& a, const Vec<N, Real>& b)
	{
		Vec<N, Real> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)+b(i);
		}
		return res;
	}
				
	//Subtract two vectors
	template<std::size_t N, typename Real>
	Vec<N, Real> operator-(const Vec<N, Real>& a, const Vec<N, Real>& b)
	{
		Vec<N, Real> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)-b(i);
		}
		return res;
	}
	
	//Negative vector
	template<std::size_t N, typename Real>
	Vec<N, Real> operator-(const Vec<N, Real>& a)
	{
		Vec<N, Real> res;
		for(int i=0; i<N;++i)
		{
			res(i)=-a(i);
		}
		return res;
	}
	
	//Multiply vector term by term
	template<std::size_t N, typename Real>
	Vec<N, Real> operator*(const Vec<N, Real>& a, const Vec<N, Real>& b)
	{
		Vec<N, Real> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)*b(i);
		}
		return res;
	}
	
	//Divide vector term by term
	template<std::size_t N, typename Real>
	Vec<N, Real> operator/(const Vec<N, Real>& a, const Vec<N, Real>& b)
	{
		Vec<N, Real> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)/b(i);
		}
		return res;
	}
	
	//Add float to a vector
	template<std::size_t N, typename Real>
	Vec<N, Real> operator+(const Vec<N, Real>& a, const Real& b)
	{
		Vec<N, Real> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)+b;
		}
		return res;
	}
	
	template<std::size_t N, typename Real>
	Vec<N, Real> operator+(const Real& b, const Vec<N, Real>& a)
	{
		return a+b;
	}
	
	//Multiply float to this vector
	template<std::size_t N, typename Real>
	Vec<N, Real> operator*(const Vec<N, Real>& a, const Real& b)
	{
		Vec<N, Real> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)*b;
		}
		return res;
	}
	
	template<std::size_t N, typename Real>
	Vec<N, Real> operator*(const Real& b, const Vec<N, Real>& a)
	{
		return a*b;
	}
	
	//Divide float to this vector
	template<std::size_t N, typename Real>
	Vec<N, Real> operator/(const Vec<N, Real>& a, const Real& b)
	{
		Vec<N, Real> res;
		for(int i=0; i<N;++i)
		{
			res(i)=a(i)/b;
		}
		return res;
	}
	
	//Get the norm of the vector
	template<std::size_t N, typename Real>
	Real length(const Vec<N, Real>& a)
	{
		return sqrt(dot(a, a));
	}
	
	//Normalize the vector
	template<std::size_t N, typename Real>
	Vec<N, Real> normalize(Vec<N, Real>& v)
	{
		v=v/Circe::length(v);
		return v;
	}
	
	//Get distance square
	template<std::size_t N, typename Real>
	Real distanceSquare(const Vec<N, Real>& a, const Vec<N, Real>& b)
	{
		return (a-b).dot(a-b);
	}
	
	//Get distance
	template<std::size_t N, typename Real>
	Real distance(const Vec<N, Real>& a, const Vec<N, Real>& b)
	{
		return sqrt(distanceSquare(a, b));
	}
	
	//Matrix multiplication, returns v*A
	template<std::size_t N, std::size_t M, typename Real>
	Vec<M, Real> operator*(const Vec<N, Real>& a, const Mat<N, M, Real>& m)
	{
		Vec<M, Real> result;
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
	}

	
	







	template<std::size_t N, typename Real = float>
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
			
			//Copy constructor
			Vec(const Vec<N, Real>& v)
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
			Vec<N>& operator=(const Vec<N, Real>& b)
			{
				for(int i=0;i<N;++i)
				{
					data[i]=b(i);
				}
				return *this;
			}
			
			//Add a vector to this one
			void operator+=(const Vec<N, Real>& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]+=b(i);
				}
			}
			
			//Subtract a vector to this one
			void operator-=(const Vec<N, Real>& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]-=b(i);
				}
			}

			//Multiply vector term by term
			void operator*=(const Vec<N, Real>& b)
			{
				for(int i=0; i<N;++i)
				{
					data[i]*=b(i);
				}
			}
			
			//Divide vector term by term
			void operator/=(const Vec<N, Real>& b)
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
			bool operator==(const Vec<N, Real>& b) const
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
			Real dot(const Vec<N, Real>& b) const
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
			
		private:
			Real data[N];
	};
	
	template<typename Real = float>
	struct Vec<2, Real>
	{
		public:
			Vec():x((Real)0.0), y((Real)0.0)
			{}
		
			Vec(const Real& x, const Real& y):x(x), y(y)
			{}
			
			//Copy constructor
			Vec(const Vec<2, Real>& v):x(v.x), y(v.y)
			{}
			
			Vec(const Real& f)
			{
				x=f;
				y=f;
			}
			
			//Copy assignment
			Vec<2, Real>& operator=(const Vec<2, Real>& b)
			{
				x=b.x;
				y=b.y;
				return *this;
			}
			
			//Add a vector to this one
			void operator+=(const Vec<2, Real>& b)
			{
				x+=b.x;
				y+=b.y;
			}
			
			//Subtract a vector to this one
			void operator-=(const Vec<2, Real>& b)
			{
				x-=b.x;
				y-=b.y;
			}

			//Multiply vector term by term
			void operator*=(const Vec<2, Real>& b)
			{
				x*=b.x;
				y*=b.y;
			}
			
			//Divide vector term by term
			void operator/=(const Vec<2, Real>& b)
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
			bool operator==(const Vec<2, Real>& b) const
			{
				return (x==b.x && y==b.y);
			}
	
			//Dot product
			Real dot(const Vec<2, Real>& b) const
			{
				return x*b.x + y*b.y;
			}
	
			//Get one element of this vector
			Real operator()(const unsigned int& index) const
			{
				assert(index<2);
				if(index==0)return x;
				if(index==1)return y;
				return 0.0f;
			}
	
			//Set one element of this vector
			Real &operator()(const unsigned int& index)
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
			
			/*Vec<2, Real> rotate(const Complex& q)
			{
				float c=q.getReal();
				float s=q.getImaginary();
				float xtemp=c*x-s*y;
				y=s*x+c*y;
				x=xtemp;
				return *this;
			}
			
			Vec<2, Real> rotateInv(const Complex& q)
			{
				float c=q.getReal();
				float s=q.getImaginary();
				float xtemp=c*x+s*y;
				y=-s*x+c*y;
				x=xtemp;
				return *this;
			}*/
			
		private:
			Real x, y;
	};
	
	template<typename Real = float>
	struct Vec<3, Real>
	{
		public:
			Vec():x((Real)0.0), y((Real)0.0f), z((Real)0.0f)
			{}
		
			Vec(const Real& x, const Real& y, const Real& z)
				:x(x), y(y), z(z)
			{}
			
			//Copy constructor
			Vec(const Vec<3, Real>& v):x(v.x), y(v.y), z(v.z)
			{}
			
			Vec(const Real& f)
			{
				x=f;
				y=f;
				z=f;
			}
			
			//Copy assignment
			Vec<3, Real>& operator=(const Vec<3, Real>& b)
			{
				x=b.x;
				y=b.y;
				z=b.z;
				return *this;
			}
			
			//Add a vector to this one
			void operator+=(const Vec<3, Real>& b)
			{
				x+=b.x;
				y+=b.y;
				z+=b.z;
			}
			
			//Subtract a vector to this one
			void operator-=(const Vec<3, Real>& b)
			{
				x-=b.x;
				y-=b.y;
				z-=b.z;
			}

			//Multiply vector term by term
			void operator*=(const Vec<3, Real>& b)
			{
				x*=b.x;
				y*=b.y;
				z*=b.z;
			}
			
			//Divide vector term by term
			void operator/=(const Vec<3, Real>& b)
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
			bool operator==(const Vec<3, Real>& b) const
			{
				return (x==b.x && y==b.y && z==b.z);
			}
	
			//Dot product
			Real dot(const Vec<3, Real>& b) const
			{
				return x*b.x + y*b.y + z*b.z;
			}
			
			//Get one element of this vector
			Real operator()(const unsigned int& index) const
			{
				assert(index<3);
				if(index==0)return x;
				if(index==1)return y;
				if(index==2)return z;
				return (Real)0.0;
			}
	
			//Set one element of this vector
			Real& operator()(const unsigned int& index)
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
			

			
		private:
			Real x, y, z, padding;
	};
	
	template<typename Real = float>
	struct Vec<4, Real>
	{
		public:
			Vec():x((Real)0.0), y((Real)0.0), z((Real)0.0), w((Real)0.0)
			{}
		
			Vec(const Real& x, const Real& y, const Real& z, 
					const Real& w):x(x), y(y), z(z), w(w)
			{}
			
			//Copy constructor
			Vec(const Vec<4, Real>& v):x(v.x), y(v.y), z(v.z), w(v.w)
			{}
			
			Vec(const Real& f)
			{
				x=f;
				y=f;
				z=f;
				w=f;
			}
			
			//Copy assignment
			Vec<4, Real>& operator=(const Vec<4, Real>& b)
			{
				x=b.x;
				y=b.y;
				z=b.z;
				w=b.w;
				return *this;
			}
			
			//Add a vector to this one
			void operator+=(const Vec<4, Real>& b)
			{
				x+=b.x;
				y+=b.y;
				z+=b.z;
				w+=b.w;
			}
			
			//Subtract a vector to this one
			void operator-=(const Vec<4, Real>& b)
			{
				x-=b.x;
				y-=b.y;
				z-=b.z;
				w-=b.w;
			}

			//Multiply vector term by term
			void operator*=(const Vec<4, Real>& b)
			{
				x*=b.x;
				y*=b.y;
				z*=b.z;
				w*=b.w;
			}
			
			//Divide vector term by term
			void operator/=(const Vec<4, Real>& b)
			{
				x/=b.x;
				y/=b.y;
				z/=b.z;
				z/=b.w;
			}

			//Add float to this vector
			void operator+=(const Real& b)
			{
				x+=b;
				y+=b;
				z+=b;
				w+=b;
			}
			
			//Subtract float to this vector
			void operator-=(const Real& b)
			{
				x-=b;
				y-=b;
				z-=b;
				w-=b;
			}

			//Multiply float to this vector
			void operator*=(const Real& b)
			{
				x*=b;
				y*=b;
				z*=b;
				w*=b;
			}
			
			//Divide float to this vector
			void operator/=(const Real& b)
			{
				x/=b;
				y/=b;
				z/=b;
				z/=b;
			}
			
			//Equality operator
			bool operator==(const Vec<4, Real>& b) const
			{
				return (x==b.x && y==b.y && z==b.z && w==b.w);
			}
	
			//Dot product
			Real dot(const Vec<4, Real>& b) const
			{
				return x*b.x + y*b.y + z*b.z + w*b.w;
			}
	
			//Get one element of this vector
			Real operator()(const unsigned int& index) const
			{
				assert(index<4);
				if(index==0)return x;
				if(index==1)return y;
				if(index==2)return z;
				if(index==3)return w;
				return (Real)0.0f;
			}
	
			//Set one element of this vector
			Real &operator()(const unsigned int& index)
			{
				assert(index<4);
				if(index==0)return x;
				if(index==1)return y;
				if(index==2)return z;
				if(index==3)return w;
				return x;
			}
			
			void reset()
			{
				x=(Real)0.0f;
				y=(Real)0.0f;
				z=(Real)0.0f;
				w=(Real)0.0f;
			}
			
		private:
			Real x, y, z, w;
	};
	
	using Vec2=Vec<2, float>;
	using Vec3=Vec<3, float>;
	using Vec4=Vec<4, float>;
	

	template<std::size_t N, std::size_t M=N, typename Real = float>
	struct Mat
	{	
		public:
			Mat()
			{	
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						//data.push_back((Real)0.0);
						data[i*M+j] = (Real)0.0;
					}
				}	
			}
			
			Mat(const std::initializer_list<Real>& values)
			{
				/*
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						data.push_back((Real)0.0);
					}
				}	*/
				
				size_t i = 0;
				size_t j = 0;
				for(Real f : values)
				{
					if(j==M)
					{
						i++;
						j=0;
					}
					data[i*M+j] =f;
					j++;
				}
			}
			
			
			Mat<N,M,Real> &operator=(const Mat<N,M,Real>& m)
			{
				for(int i=0;i<N;++i)
				{
					for(int j = 0; j<M; ++j)
					{
						data[i*M+j] = m(i,j);
					}
				}
				return *this;
			}
			
			//Turn the matrix to I
			void setIdentity()
			{
				for(int i = 0; i<min(N, M); ++i)
				{
					data[i*M+i]=(Real)1.0;
				}	
			}
			
			//Set all the coeffs to 0
			void reset()
			{
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						data[i*M+j]=(Real)0.0;
					}
				}	
			}
			
			Mat<M,N,Real> getTranspose() const
			{
				Mat<M,N> result;
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						result(j,i)=get(i,j);
					}
				}	
				return result;
			}
			
			//Matrix multiplication to matrix
			template<std::size_t O>
			Mat<N,O,Real> operator*(const Mat<M,O,Real>& m) const
			{
				Mat<N,O> result;
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<O; ++j)
					{
						Real value = (Real)0.0;
						for(int k=0;k<M; ++k)
						{
							value+=get(i,k)*m(k,j);
						}
						result(i,j)=value;
					}
				}	
				return result;
			}
			
			//Matrix multiplication to vector, returns A*v
			Vec<N,Real> operator*(const Vec<M,Real>& v) const
			{
				Vec<N,Real> result;
				for(int i=0; i<N;++i)
				{
					Real value = (Real)0.0;
					for(int j = 0; j<M; ++j)
					{
						value+=v(j)*get(i,j);
					}
					result(i)=value;
				}
				return result;
			}
			
			//Get the matrix multiplied by a scalar
			Mat<N,M,Real> operator*(const Real& f) const
			{
				Mat<N,M,Real> result;
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						result(i,j)=get(i,j)*f;
					}
				}	
				return result;	
			}
			
			Mat<N,M,Real> operator/(const Real& f) const
			{
				Mat<N,M,Real> result;
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						result(i,j)=get(i,j)/f;
					}
				}	
				return result;	
			}
			
			void operator+=(const Mat<N,M,Real>& m)
			{
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						data[i*M+j] += m(i,j);
					}
				}	
			}
			
			Mat<N,M,Real> operator+(const Mat<N,M,Real>& m) const
			{
				Mat<N,M,Real> res;
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						res(i,j)=get(i,j)+m(i,j);
					}
				}
				return res;
			}
			
			void operator-=(const Mat<N,M,Real>& m)
			{
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						data[i*M+j]-=m(i,j);
					}
				}	
			}
			
			//Multiply by a scalar
			void operator*=(const Real& f)
			{
				for(int i = 0; i<N; ++i)
				{
					for(int j = 0; j<M; ++j)
					{
						data[i*M+j]*=f;
					}
				}
			}
			
			//Get the coefficient at row i and column j
			Real operator()(const int& i, const int& j) const
			{
				return data[i*M+j];
			}
			
			//Set the coefficient at row i and column j
			Real &operator()(const int& i, const int& j)
			{
				return data[i*M+j];;
			}
			
			Real* getData()
			{
				static Real res[N*M];
				int k=0;
				for(int i=0; i<M; i++)
				{
					for(int j=0; j<N; j++)
					{
						res[k] = get(j,i);
						k++;
					}
				}
				return res;
			}
			
			Real get(const int& i, const int& j) const
			{
				return data[i*M+j];
			}
			
			Vec<M, Real> getRow(const int& i) const
			{
				Vec<N, Real> row;
				for(int j=0; j<M; j++)
				{
					row(j) = get(i,j);
				}
				return row;
			}
			
			Vec<N, Real> getColumn(const int& j) const
			{
				Vec<N, Real> column;
				for(int i=0; i<N; i++)
				{
					column(i) = get(i,j);
				}
				return column;
			}
			
			void setRow(const int& i, const Vec<M, Real>& row)
			{
				for(int j=0; j<M; j++)
				{
					data[i*M+j] = row(j);
				}
			}
			
			void setColumn(const int& j, const Vec<N, Real>& column)
			{
				for(int i=0; i<N; i++)
				{
					data[i*M+j] = column(i);
				}
			}
				
		private:
			//std::vector<Real> data;
			Real data[N*M];
	};
	
	using Mat44=Mat<4, 4, float>;

	struct Complex
	{
		public:
			inline Complex(const float& angle):c(cos(angle)), s(sin(angle))
			{}
			
			inline Complex(const float& real, const float& imaginary):
							c(real), s(imaginary)
			{}
	
			inline	Complex(const Mat44& m):Complex(m.get(0,0), m.get(0,1))
			{}

			inline float getAngle() const
			{
				return atan2(s,c);
			}
			
			inline Complex operator*(const Complex& q) const
			{
				float a=c;float b=s;
				float c=q.c;float d=q.s;
				return Complex(a*c-b*d, b*c+a*d);
			}
			
			inline Complex operator=(const Complex& q)
			{
				c=q.c;
				s=q.s;
				return *this;
			}
			
			inline float length() const
			{
				return sqrt(c*c+s*s);
			}
			
			inline Complex normalize()
			{
				float length = (*this).length();
				c/=length;
				s/=length;
				return *this;
			}
			
			inline Complex conjugate()
			{
				s*=-1.0f;
				return *this;
			}
			
			inline float getReal() const
			{
				return c;
			}
			
			inline float getImaginary() const
			{
				return s;
			}
			
			inline void addAngle(const float& dtheta, const float& dt)
			{
				Complex q(dtheta);
				float ctemp = c*q.c*dt - s*q.s*dt;
				s = c*q.s*dt + s*q.c*dt;
				c=ctemp;
			}
	
		private:
			float c,s;
	};
	
	
	class Quaternion
	{
		public:
			inline Quaternion():w(1.0f), x(0.0f), y(0.0f), z(0.0f)
			{}
			
			inline Quaternion(const float& w, const float& x, 
							  const float& y, const float& z):
								w(w), x(x), y(y), z(z)
			{}
			
			inline Quaternion(const float& roll, const float& pitch, 
								const float& yaw)
			{
				float cy = cos(yaw*0.5f);
				float sy = sin(yaw*0.5f);
				float cp = cos(pitch*0.5f);
				float sp = sin(pitch*0.5f);
				float cr = cos(roll*0.5f);
				float sr = sin(roll*0.5f);
				
				w = cy*cp*cr+sy*sp*sr;
				x = cy*cp*sr-sy*sp*cr;
				y = sy*cp*sr+cy*sp*cr;
				z = sy*cp*cr-cy*sp*sr;
				
				//Source: Wikipedia, conversion between 
				// quaternion and Euler angles
			}
	
			inline Quaternion(const Mat44& m)
			{
				float trace = m.get(0, 0) + m.get(1, 1) + m.get(2, 2);
				
				if(trace > 0)
				{
					float s = 0.5f / (float)std::sqrt(trace+ 1.0f);
					w = 0.25f / s;
					x = (m.get(1, 2) - m.get(2, 1)) * s;
					y = (m.get(2, 0) - m.get(0, 2)) * s;
					z = (m.get(0, 1) - m.get(1, 0)) * s;
				}
				else
				{
					if(m.get(0, 0) > m.get(1, 1) && m.get(0, 0) > m.get(2, 2))
					{
						float s = 2.0f * (float)std::sqrt(1.0f + m.get(0, 0) - m.get(1, 1) - m.get(2, 2));
						w = (m.get(1, 2) - m.get(2, 1)) / s;
						x = 0.25f * s;
						y = (m.get(1, 0) + m.get(0, 1)) / s;
						z = (m.get(2, 0) + m.get(0, 2)) / s;
					}
					else if(m.get(1, 1) > m.get(2, 2))
					{
						float s = 2.0f * (float)std::sqrt(1.0f + m.get(1, 1) - m.get(0, 0) - m.get(2, 2));
						w = (m.get(2, 0) - m.get(0, 2)) / s;
						x = (m.get(1, 0) + m.get(0, 1)) / s;
						y = 0.25f * s;
						z = (m.get(2, 1) + m.get(1, 2)) / s;
					}
					else
					{
						float s = 2.0f * (float)std::sqrt(1.0f + m.get(2, 2) - m.get(0, 0) - m.get(1, 1));
						w = (m.get(0, 1) - m.get(1, 0) ) / s;
						x = (m.get(2, 0) + m.get(0, 2) ) / s;
						y = (m.get(1, 2) + m.get(2, 1) ) / s;
						z = 0.25f * s;
					}
				}

				float length = (float)std::sqrt(x * x + y * y + z * z + w * w);
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
				float w0 = w*q.w - x*q.x - y*q.y - z*q.z;
				float x0 = w*q.x + x*q.w + y*q.z - z*q.y;
				float y0 = w*q.y - x*q.z + y*q.w + z*q.x;
				float z0 = w*q.z + x*q.y - y*q.x + z*q.w;
				w = w0;
				x = x0;
				y = y0;
				z = z0;
			}
			
			inline void addAngle(const float& roll, const float& pitch, 
								 const float& yaw, const float& dt)
			{
				
				Quaternion q = (*this)*Quaternion(0.0f, 
												  -roll*0.5f*dt, 
												  -pitch*0.5f*dt, 
												  -yaw*0.5f*dt);
				w+=q.w;
				x+=q.x;
				y+=q.y;
				z+=q.z;
						
				normalize();
			}
			
			inline float length() const
			{
				return sqrt(w*w+x*x+y*y+z*z);
			}
			
			inline Quaternion normalize()
			{
				float length = (*this).length();
				w/=length;
				x/=length;
				y/=length;
				z/=length;

				return *this;
			}
			
			inline Quaternion conjugate()
			{
				x*=-1.0f;
				y*=-1.0f;
				z*=-1.0f;

				return *this;
			}
			
			inline Quaternion getConjugate() const
			{
				return Quaternion(w, -x, -y, -z);
			}
			
			template<typename Real>
			Vec<3, Real> rotate(const Vec<3, Real>& v2)
			{
				Quaternion v((Real)0.0, v2(0), v2(1), v2(2));
				Quaternion p(*this);
				p.normalize();
				Quaternion pConj=p.getConjugate();
				Quaternion result = p*v*pConj;
				Vec<3, Real> v3;
				v3(0)=result.getX();
				v3(1)=result.getY();
				v3(2)=result.getZ();
				return v3;
			}
			
			template<typename Real>
			Vec<3, Real> rotateInv(const Vec<3, Real>& p)
			{
				return getConjugate().rotate(p);
			}

			inline float getW() const
			{
				return w;
			}
			
			inline float getX() const
			{
				return x;
			}
			
			inline float getY() const
			{
				return y;
			}
			
			inline float getZ() const
			{
				return z;
			}
	
	
	

		private:
			float x,y,z,w;
	};
	
	static Mat44 positionMatrix(const float& x, const float& y, 
								const float& z)
	{
		Mat44 m;
		m.setIdentity();
		m(0,3)=x;
		m(1,3)=y;
		m(2,3)=z;
		return m;
	}
	
	static Mat44 positionMatrix(const Vec2& v)
	{
		return positionMatrix(v(0), v(1), 0.0f);
	}
	
	static Mat44 positionMatrix(const Vec3& v)
	{
		return positionMatrix(v(0), v(1), v(2));
	}
	
	static Mat44 scaleMatrix(const float& x, const float& y, 
							 const float& z)
	{
		Mat44 m;
		m.setIdentity();
		m(0,0)=x;
		m(1,1)=y;
		m(2,2)=z;

		return m;
	}

	static Mat44 scaleMatrix(const Vec<2>& v)
	{
		return scaleMatrix(v(0), v(1), 1.0f);
	}
	
	static Mat44 scaleMatrix(const Vec<3>& v)
	{
		return scaleMatrix(v(0), v(1), v(2));
	}
	
	//Get the rotation matrix
	static Mat44 rotationMatrix(const Complex& q)
	{
		float c = q.getReal();
		float s = q.getImaginary();
		
		Mat44 m;
		
		m(0,0)=c;		m(0,1)=s;		m(0,2)=0.0f;	m(0,3)=0.0f;
		m(1,0)=-s;		m(1,1)=c;		m(1,2)=0.0f;	m(1,3)=0.0f;
		m(2,0)=0.0f;	m(2,1)=0.0f;	m(2,2)=1.0f;	m(2,3)=0.0f;
		m(3,0)=0.0f;	m(3,1)=0.0f;	m(3,2)=0.0f;	m(3,3)=1.0f;
		
		return m;
	}
	
	static Mat44 rotationMatrix(const Quaternion& q)
	{
		float x = q.getX();
		float y = q.getY();
		float z = q.getZ();
		float w = q.getW();
		
		Mat44 m;
		
		m(0,0)=1.0f-(2.0f*y*y+2.0f*z*z);	m(0,1)=2.0f*x*y+2.0f*z*w;			m(0,2)=2.0f*x*z-2.0f*y*w;			m(0,3)=0.0f;
		m(1,0)=2.0f*x*y-2.0f*z*w;			m(1,1)=1.0f-(2.0f*x*x+2.0f*z*z);	m(1,2)=2.0f*y*z+2.0f*x*w;			m(1,3)=0.0f;
		m(2,0)=2.0f*x*z+2.0f*y*w;			m(2,1)=2.0f*y*z-2.0f*x*w;			m(2,2)=1.0f-(2.0f*x*x+2.0f*y*y);	m(2,3)=0.0f;
		m(3,0)=0.0f;						m(3,1)=0.0f;						m(3,2)=0.0f;						m(3,3)=1.0f;
		
		return m;
	}
	
	static Mat44 rotationMatrix(const Vec<2>& fwdAxis)
	{
		Vec<2> f = fwdAxis;
		normalize(f);
		
		Mat44 rotationMatrix;

		rotationMatrix(0,0) = f(0);
		rotationMatrix(0,1) = f(1);
		rotationMatrix(1,0) = -f(0);
		rotationMatrix(0,1) = f(1);
		
		rotationMatrix(2,2) = 1.0f;
		rotationMatrix(3,3) = 1.0f;
		return rotationMatrix;
	}
	
	static Mat44 rotationMatrix(const Vec<3>& leftAxis, 
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
		
		Mat44 rotationMatrix;
		for(int j = 0; j<3; j++)
		{
			rotationMatrix(0,j) = l(j);
			rotationMatrix(1,j) = f(j);
			rotationMatrix(2,j) = u(j);
		}
		rotationMatrix(3,3) = 1.0f;
		return rotationMatrix;
	}
/*	
	
*/	
	static Mat44 orthoProjection(const float& width, const float& height, const float& nearField, const float& farField)
	{
		float left=-width/2.0f;
		float right=width/2.0f;
		float bottom=-height/2.0f;
		float top=height/2.0f;
		Mat44 m;
		m(0,0)=2.0f/width;
		m(1,1)=2.0f/height;
		m(2,2)=2.0f/(farField-nearField);
		m(3,3)=1.0f;
		
		m(0,3)=-(right+left)/(right-left);
		m(1,3)=-(top+bottom)/(top-bottom);
		m(2,3)=-(farField+nearField)/(farField-nearField);
		
		return m;
	}
	
	static Mat44 perspectiveProjection(const float& fieldOfView, const float& aspectRatio, const float& nearField, const float& farField)
	{
		Mat44 m;
		float tanHalfFov=std::tan(fieldOfView*3.141593/(2.0*180));
		float range = farField-nearField;
		m(0,0) = 1/(aspectRatio*tanHalfFov);
		m(1,1) = 1/(tanHalfFov);
		m(2,2) = (nearField+farField)/range;
		m(2,3) = -2*farField*nearField/range;
		m(3,2) = 1;
		return m;
	}



	struct Transform3
	{
		public:
			inline Transform3(const Vec3& position0 = Vec3(0.0f,0.0f,0.0f),
					   const Quaternion& rotation = Quaternion(), 
					   const Vec3& scale = Vec3(1.0f, 1.0f, 1.0f)) :
						position(position0), rotation(rotation),
						scale(scale)
			{}

			
			inline Transform3(const Transform3& transform)
			{
				position = transform.position;
				rotation = transform.rotation;
				scale = transform.scale;
			}
			
			inline Transform3& operator=(const Transform3& transform)
			{	
				position = transform.position;
				rotation = transform.rotation;
				scale = transform.scale;
				return *this;
			}

			inline Vec3& getPosition()
			{
				return position;
			}
			
			inline Vec3 getPosition() const
			{
				return position;
			}

			inline void setPosition(const Vec3& newPosition)
			{				
				position = newPosition;			
			}
			
			inline void setPosition(const float& x, const float& y, 
									const float& z)
			{
				position(0) = x;
				position(1) = y;
				position(2) = z;
			}
	
			inline Quaternion& getRotation()
			{
				return rotation;
			}

			inline Quaternion getRotation() const
			{
				return rotation;
			}		

			/*inline void setRotation(const Vec2& fwdAxis)
			{
				rotation(rotationMatrix(fwdAxis));
			}
			
			inline void setRotation(const Vec3& leftAxis, 
									const Vec3& fwdAxis)
			{
				rotation(rotationMatrix(leftAxis, fwdAxis));
			}*/

			inline Vec3& getScale()
			{
				return scale;
			}
			
			inline Vec3 getScale() const
			{
				return scale;
			}

			inline void setScale(const Vec3& newScale)
			{
				scale = newScale;
			}

			inline void setScale(const float& x, const float& y, 
										const float& z)
			{
				scale(0) = x;
				scale(1) = y;
				scale(2) = z;
			}	
			
			inline void rotate(const Vec3& eulerAngles, const float& dt)
			{
				rotation.addAngle(eulerAngles(0), eulerAngles(1), 
								eulerAngles(2), dt);
			}
			
			inline void translate(const Vec3& v)
			{
				position += v;
			}
			
			inline void resize(const float& scaleRatio)
			{
				scale*=scaleRatio;
			}
			
			inline Mat44 getTransformMatrix() const
			{
				Mat44 res = (positionMatrix(position)) 
						   * (rotationMatrix(rotation)) 
						   * (scaleMatrix(scale));

				if(std::shared_ptr<Transform3> parent = m_parent.lock())
				{
					return parent->getTransformMatrix()*res;
				}
				else
				{
					return res;
				}
			}
			
			inline void attachTo(const std::shared_ptr<Transform3>& parent)
			{
				m_parent = parent;
			}
		
			inline void detachFrom(const std::shared_ptr<Transform3>& 
									parent)
			{
				m_parent.reset();
			}
			

			inline std::weak_ptr<Transform3> getParent()
			{
				return m_parent;
			}	

		private:
			/** Attitude stored in LOCAL reference frame */
			Vec3 position;
			Quaternion rotation;
			Vec3 scale;
			std::weak_ptr<Transform3> m_parent;
	};
	

	inline std::ostream& operator<<(std::ostream &strm, const Complex &q)
	{
		strm << "[" << q.getReal() << " + i" << q.getImaginary();
		return strm << "]";
	}
	
	
	inline std::ostream& operator<<(std::ostream &strm, const Quaternion &q)
	{
		strm << "[" << q.getW() << ", " << q.getX() 
			 << ", " << q.getY() << ", " << q.getZ();
		return strm << "]";
	}
	
	template<std::size_t N, typename Real>
	std::ostream& operator<<(std::ostream &strm, const Vec<N, Real> &v)
	{
		strm << "[" << v(0);
		for(int i=1; i<N;++i){
			strm << ", " << v(i);
		}
		return strm << "]";
	}

	template<std::size_t N, std::size_t M, typename Real>
	std::ostream& operator<<(std::ostream &strm, const Mat<N, M, Real> &m)
	{
		std::cout.precision(3);
		for(int i=0; i<N;++i){
			strm << "|" << m(i,0);
			for(int j=1; j<M; ++j)
			{
				strm << ",\t" << m(i,j);
			}
			strm << "|" << std::endl;
		}
		return strm;
	}
	
}
