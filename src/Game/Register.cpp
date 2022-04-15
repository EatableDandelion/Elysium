#include "Game/Register.h"

namespace Elysium
{
	void Register::setVariable(const std::string& name,
								const std::vector<Real> value)
	{
		std::size_t nameID = CIRCE_STRING_ID(name);

		if(hasVariable(nameID))
		{
			unsigned int size = m_positions.at(nameID).size;

			if(size != value.size()) 
				CIRCE_ERROR("The uniform "+name+" has changed size.");

			unsigned int offset = m_positions.at(nameID).offset;
			
			for(int i = 0; i < size; i++)
				m_values[offset + i] = value[i];
		}
		else
		{
			BufferPosition position;

			position.offset = m_values.size();
			position.size 	= value.size();

			m_positions.insert(
				std::pair<std::size_t, BufferPosition>(nameID, position));

			for(int i = 0; i < value.size(); i++)
				m_values.push_back(value[i]);
		}
	}

	void Register::setVariable(const std::string& name, 
								const Real& value)
	{
		setVariable(name, std::vector<Real>({value}));	
	}

	void Register::setVariable(const std::string& name, 
					const Circe::Vec2& value)
	{
		setVariable(name, std::vector<Real>({value(0), value(1)}));	
	}

	void Register::setVariable(const std::string& name, 
					const Circe::Vec3& value)
	{
		setVariable(name, std::vector<Real>({value(0),value(1),value(2)}));
	}

	void Register::setVariable(const std::string& name,const Mat& value)
	{
		std::vector<Real> v(
				{value(0,0),value(0,1),value(0,2),value(0,3),
				 value(1,0),value(1,1),value(1,2),value(1,3),
			     value(2,0),value(2,1),value(2,2),value(2,3),
			     value(3,0),value(3,1),value(3,2),value(3,3)});

		setVariable(name, v);	
	}

	std::vector<Real> Register::operator()(const std::size_t name) const
	{
		std::vector<Real> uniform;

		if(hasVariable(name))
		{	
			unsigned int size = m_positions.at(name).size;
			unsigned int offset = m_positions.at(name).offset;
		

			for(int i = 0; i < size; i++)
				uniform.push_back(m_values[offset + i]);
		}

		return uniform;
	}

	bool Register::hasVariable(const std::string& name) const
	{
		return hasVariable(CIRCE_STRING_ID(name));
	}		

	bool Register::hasVariable(const std::size_t nameID) const
	{
		return m_positions.count(nameID);
	}
}
