#pragma once

#include <string>
#include <Circe/Circe.h>
#include "Circe/Event.h"

namespace Elysium
{
	

	class Register
	{
		struct RegisterKey
		{
			unsigned int size;
			unsigned int offset;
			std::shared_ptr<Emitter<std::vector<Real>>> emitter;

			void addListener(const Listener<std::vector<Real>> listener)
			{
				if(!emitter)
				{
					emitter=std::make_shared<Emitter<std::vector<Real>>>();
				}
		
				emitter->addListener(listener);
			}

			void callListeners(const std::vector<Real>& values)
			{
				if(emitter)
				{
					std::vector<Real> vec(values.begin()+offset,
										  values.begin()+offset+size);
					emitter->broadcast(vec);
				}
			}
		};	

		public:
			void setVariable(const std::string& name,
							const std::vector<Real> value)
			{
				std::size_t nameID = CIRCE_STRING_ID(name);

				if(hasVariable(nameID))
				{
					RegisterKey key = m_positions.at(nameID);
					unsigned int size = key.size;

					if(size != value.size()) 
						CIRCE_ERROR("The variable "
									+name
									+" has changed size.");

					unsigned int offset = key.offset;
					
					for(int i = 0; i < size; i++)
						m_values[offset + i] = value[i];

					key.callListeners(m_values);
				}
				else
				{
					RegisterKey position;

					position.offset = m_values.size();
					position.size 	= value.size();

					m_positions.insert(
						std::pair<std::size_t, RegisterKey>
								 (nameID, position));

					for(int i = 0; i < value.size(); i++)
						m_values.push_back(value[i]);
				}
			}

			void setVariable(const std::string& name, const Real& value)
			{
				setVariable(name, std::vector<Real>({value}));	
			}

			void setVariable(const std::string& name, 
							 const Circe::Vec2& value)
			{
				setVariable(name, std::vector<Real>({value(0), value(1)}));	
			}

			void setVariable(const std::string& name, 
							 const Circe::Vec3& value)
			{
				setVariable(name, std::vector<Real>
							({value(0),value(1),value(2)}));
			}

			void setVariable(const std::string& name, 
							 const Mat& value)
			{
				std::vector<Real> v(
						{value(0,0),value(0,1),value(0,2),value(0,3),
						 value(1,0),value(1,1),value(1,2),value(1,3),
						 value(2,0),value(2,1),value(2,2),value(2,3),
						 value(3,0),value(3,1),value(3,2),value(3,3)});

				setVariable(name, v);	
			}

			std::vector<Real> operator()(const std::size_t nameID) const
			{
				std::vector<Real> uniform;

				if(hasVariable(nameID))
				{	
					unsigned int size = m_positions.at(nameID).size;
					unsigned int offset = m_positions.at(nameID).offset;
				

					for(int i = 0; i < size; i++)
						uniform.push_back(m_values[offset + i]);
				}

				return uniform;
			}

			std::vector<Real> getVariable(const std::string& name)
			{
				return (*this)(CIRCE_STRING_ID(name));
			}

			bool hasVariable(const std::string& name) const
			{
				return hasVariable(CIRCE_STRING_ID(name));
			}

			bool hasVariable(const std::size_t nameID) const
			{
				return m_positions.count(nameID);
			}

			void addListener(const std::string& name,
							 const Listener<std::vector<Real>> lisnr)
			{
				if(hasVariable(name))
				{
					m_positions[CIRCE_STRING_ID(name)].addListener(lisnr);
				}
			}

		private:
			std::vector<Real> m_values;
			std::unordered_map<std::size_t, RegisterKey> m_positions;
	};
}
