#pragma once
#include <assert.h>
#include <iostream>
#include <ostream>
#include <math.h>
#include <memory>
#include <vector>
//#include "Profile.h"

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
			Mat();

			Mat(const std::size_t newM, const std::size_t newN);
		
			void init(const std::size_t newM, const std::size_t newN);

			Mat(const std::size_t M, const std::size_t N,
				const std::initializer_list<Real>& values);

			Mat(const std::initializer_list<Mat>& values);
			
			Mat& operator=(const Mat& m);
			
			//Matrix multiplication to matrix
			Mat operator*(const Mat& m) const;
				
			Mat transposeMult(const Mat& m) const;
			
			Mat multTranspose(const Mat& m) const;

			//Get the matrix multiplied by a scalar
			Mat operator*(const Real& f) const;

			Mat operator/(const Real& f) const;
			
			void operator+=(const Mat& m);
			
			Mat operator+(const Mat& m) const;
				
			Mat operator-(const Mat& m) const;

			void operator-=(const Mat& m);
			
			//Multiply by a scalar
			void operator*=(const Real& f);

			/** Solve the LU factorization of Ax = b with b = this */
			Mat operator/(Mat& A) const;

			Mat solveLU(const Mat& A);
					
			void initQR();

			Mat getColumn(const int index) const;

			Mat backSolve(const Mat& A) const;

			Mat solveQR(Mat& A) const;

			//Get the coefficient at row i and column j
			Real operator()(const int& i, const int& j = 0) const;
			
			//Set the coefficient at row i and column j
			Real& operator()(const int& i, const int& j = 0);

			void set(const int& i, const int& j, const Real& value);

			std::size_t getNbRows() const;
			
			std::size_t getNbCols() const;
			
		private:
			std::vector<Real> data;
			std::size_t M,N;
			std::shared_ptr<Mat> Q;
			std::shared_ptr<Mat> R;
			bool hasQR = false;
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
			Complex(const Real& angle);
			
			Complex(const Real& real, const Real& imaginary);
	
			Complex(const Mat& m);

			Real getAngle() const;
			
			Complex operator*(const Complex& q) const;
			
			Complex operator=(const Complex& q);
			
			Real length() const;
			
			Complex normalize();
			
			Complex conjugate();
			
			Real getReal() const;
			
			Real getImaginary() const;
			
			void addAngle(const Real& dtheta);
	
		private:
			Real c,s;
	};
	
	
	class Quaternion
	{
		public:
			Quaternion();
			
			Quaternion(const Real& w, const Real& x, 
							  const Real& y, const Real& z);
			
			Quaternion(const Real& roll, const Real& pitch, 
								const Real& yaw);
	
			Quaternion(const Mat& m);

			Quaternion operator=(const Quaternion& q);
			
			Quaternion operator*(const Quaternion& q) const;
			
			void operator*=(const Quaternion& q);
						
			void addAngle(const Real& roll, const Real& pitch, 
								 const Real& yaw);
						
			Real length() const;
						
			Quaternion normalize();
						
			Quaternion conjugate();
						
			Quaternion getConjugate() const;
						
			Vec<2> rotate(const Vec<2>& v2) const;
			
			Vec<3> rotate(const Vec<3>& v2) const;
						
			Vec<2> rotateInv(const Vec<2>& v2) const;
		
			Vec<3> rotateInv(const Vec<3>& v2) const;
			
			/** https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles  */
			Vec<3> toEulerAngle() const;
			
			Real angleWith(const Quaternion& other);
			
			void lookAt(const Vec<2>& lookAt);
			
			void lookAt(const Vec<3>& lookAt, const Vec<3>& up0);
			
			Real getW() const;
						
			Real getX() const;
						
			Real getY() const;
						
			Real getZ() const;
				
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
			Trans(const Vec<N>& position0 = Vec<N>(),
					   const Quaternion& rotation = Quaternion(), 
					   const Vec<N>& scale = Vec<N>(1.0)) :
						position(position0), rotation(rotation),
						scale(scale)
			{}

			
			Trans(const Trans<N>& transform)
			{
				position = transform.position;
				rotation = transform.rotation;
				scale = transform.scale;
			}
			
			Trans<N>& operator=(const Trans<N>& transform)
			{	
				position = transform.position;
				rotation = transform.rotation;
				scale = transform.scale;
				return *this;
			}

			Vec<N>& getPosition()
			{
				return position;
			}
			
			Vec<N> getPosition() const
			{
				return position;
			}

			void setPosition(const Vec<N>& newPosition)
			{				
				position = newPosition;			
			}
			
			Quaternion& getRotation()
			{
				return rotation;
			}

			Quaternion getRotation() const
			{
				return rotation;
			}		

			void setRotation(const Quaternion& other)
			{
				rotation = other;
			}

			Vec<N>& getScale()
			{
				return scale;
			}
			
			Vec<N> getScale() const
			{
				return scale;
			}

			void setScale(const Vec<N>& newScale)
			{
				scale = newScale;
			}
			
			void rotate(const Real angle)
			{
				rotation.addAngle(0.0, 0.0, angle);
			}

			void rotate(const Vec3& eulerAngles)
			{
				rotation.addAngle(eulerAngles(0), eulerAngles(1), 
								eulerAngles(2));
			}
			
			void translate(const Vec<N>& v)
			{
				position += v;
			}
			
			void resize(const Real& scaleRatio)
			{
				scale*=scaleRatio;
			}
			
			Mat getTransformMatrix() const
			{
				Mat res = (positionMatrix(position)) 
						   * (rotationMatrix(rotation)) 
						   * (scaleMatrix(scale));

				return res;
			}

			Vec<N> toGlobal(const Vec<N>& vec, const bool translate)
			{
				Vec<N> res = rotation.rotateInv(vec);

				if(translate)
					res = res + position;

				return res;
			}

			Vec<N> toLocal(const Vec<N>& vec, const bool translate)
			{
				Vec<N> res = vec;

				if(translate)
					res = res - position;

				res = rotation.rotate(res);

				return res;
			}

			Real angleWith(const Trans<N>& other)
			{
				return rotation.angleWith(other.rotation);
			}

			void lookAt(const Vec<2>& location)
			{
				rotation.lookAt(location-position);
			}
			
			void lookAt(const Vec<3>& location)
			{
				rotation.lookAt(location-position, Vec<3>(0,0,1));
			}

		
			/** Modify the transform such that one end is a location p1 
			  	and the other end is at location p2 (useful for lines)
			 */	
			void lineUp(const Vec<N>& p1, const Vec<N>& p2, 
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
