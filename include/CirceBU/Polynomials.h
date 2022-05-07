#pragma once

#include <iostream>
#include <map>
#include <math.h>

namespace Circe
{
	class Polynomial
	{
		public:
			Polynomial();
			
			Polynomial(const std::initializer_list<float>& values);
			
			float operator()(const float& x) const;
			
			Polynomial operator+(const Polynomial& other) const;
			
			void operator+=(const Polynomial& other);
			
			Polynomial operator-(const Polynomial& other) const;
			
			Polynomial operator*(const Polynomial& other) const;
			
			Polynomial operator*(const float& f) const;
			
			void operator*=(const float& f);
			
			void setCoefficient(const int& deg, const float& value);
			
			float getCoefficient(const int& deg) const;
			
			int getDegree() const;
			
			bool hasDegree(const int& deg) const;
			
			void raisePower(const int& power);
			
			
		private:
			std::map<int, float> data;
			int degree = 0;
			
			void eraseCoefficient(const int& deg);
	};
	
	std::ostream& operator<<(std::ostream &strm, const Polynomial& P);
	
	Polynomial LegendrePolynomial(const int& degree);
}