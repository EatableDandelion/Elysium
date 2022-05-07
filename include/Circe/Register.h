#pragma once

#include <string>
#include "Utils.h"
#include "Event.h"

namespace Circe
{
	class Register
	{
		struct RegisterKey
		{
			unsigned int size;
			unsigned int offset;
			std::shared_ptr<Emitter<std::vector<Real>>> emitter;

			void addListener(const Listener<std::vector<Real>> listener);

			void callListeners(const std::vector<Real>& values);
		};	

		public:
			void setVariable(const std::string& name,
							 const std::vector<Real> value);
			
			void setVariable(const std::string& name, const Real& value);
			
			void setVariable(const std::string& name, 
							 const Circe::Vec2& value);
			
			void setVariable(const std::string& name, 
							 const Circe::Vec3& value);
			
			void setVariable(const std::string& name, 
							 const Mat& value);
			
			std::vector<Real> operator()(const std::string& name) const;

			std::vector<Real> getVariable(const std::size_t nameID) const;
			
			bool hasVariable(const std::string& name) const;
			
			bool hasVariable(const std::size_t nameID) const;
			
			void addListener(const std::string& name,
							 const Listener<std::vector<Real>> lisnr);
			
		private:
			std::vector<Real> m_values;
			std::unordered_map<std::size_t, RegisterKey> m_positions;
	};
}
