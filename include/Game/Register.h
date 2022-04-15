#pragma once

#include <string>
#include <Circe/Circe.h>

namespace Elysium
{
	class Register
	{
		struct BufferPosition
		{
			unsigned int size;
			unsigned int offset;
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

			std::vector<Real> operator()(const std::size_t nameID) const;

			std::vector<Real> getVariable(const std::string& name)
			{
				return (*this)(CIRCE_STRING_ID(name));
			}

			bool hasVariable(const std::string& name) const;

			bool hasVariable(const std::size_t nameID) const;

		private:
			std::vector<Real> m_values;
			std::unordered_map<std::size_t, BufferPosition> m_positions;
	};
}
