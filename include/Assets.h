#pragma once
#include <string>
#include <Circe/Circe.h>
#include <iostream>
#include <unordered_map>
#include <map>
#include <memory>

namespace Elysium
{
	template<typename Resource, typename Loader>
	class ResourceManager
	{
		public:
			ResourceManager(const std::string& folderLocation) 
				: m_loader(Loader()), m_folderLocation(folderLocation)
			{}
			
			virtual ~ResourceManager()
			{
				for(auto &resource : m_resources)
				{
					m_loader.unload(resource.second);					
					CIRCE_INFO("Resource "+resource.first+" unloaded.");
				}
			}
			
			template<typename... Args>
			Resource getResource(const std::string& fileName, 
								 Args&&... args)
			{
				std::string name = m_folderLocation+fileName;
				if(m_resources.count(name))
				{
					CIRCE_INFO("Resource "+fileName+" retrieved.");
					return m_resources[name];
				}
				else
				{
					Resource resource = m_loader.load(name, 
										std::forward<Args>(args)...);
					m_resources.insert(std::pair<std::string, Resource>
										(name, resource));
					CIRCE_INFO("Resource "+fileName+" loaded.");
					return resource;
				}
			}
		
		private:
			std::string m_folderLocation;
			Loader m_loader;
			std::unordered_map<std::string, Resource> m_resources;
	};
}
