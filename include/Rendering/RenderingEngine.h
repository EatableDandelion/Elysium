#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <Circe/Circe.h>
#include "Rendering/Shader.h"
#include "Rendering/Model.h"
#include "Game/Camera.h"
#include "Rendering/RenderingPass.h"
#include "Game/Input.h"
#include "Circe/Event.h"

namespace Elysium
{
	typedef std::type_index PassID;

	class RenderingNode
	{
		public:
			RenderingNode();

			RenderingNode(const Shader& shader);
		

			/** Add the pass rendered after this one */
			template<typename Pass, typename... Args>
			void addPass(const Shader& shader, Args&&... args) 
			{
				if(m_next == nullptr)
				{
					m_next = std::make_shared<RenderingNode>(shader);
					m_next->newPass<Pass>(std::forward<Args>(args)...);
				}
				else
				{
					m_next->addPass<Pass>(shader, 
										  std::forward<Args>(args)...);
				}
			}

			template<typename Pass>
			std::shared_ptr<Pass> getPass() const
			{
				if(getID() == std::type_index(typeid(Pass)))
				{
					return std::dynamic_pointer_cast<Pass>(m_pass);
				}

				if(m_next == nullptr) 
					return nullptr;

				return m_next->getPass<Pass>();
			}

			/** Init this pass, calls child pass */
			virtual void init(RenderContext& renderer);

			/** Draw this pass, calls child pass */
			virtual void draw(RenderContext& renderer);

			virtual void draw(Model& model,
							  const Transform transform, 
							  RenderContext& renderer);

			template<typename Pass, typename... Args>
			void newPass(Args&&... as)
			{
				m_pass = std::make_shared<Pass>(std::forward<Args>(as)...);
				m_id = std::type_index(typeid(Pass));
			}
	
		protected:		
			std::shared_ptr<RenderingNode> m_next;
			std::shared_ptr<RenderingPass> m_pass;
			Shader m_shader;

		private:
			PassID m_id;

			PassID getID() const;
	};

	class Renderer
	{
		public:
			Renderer(const std::string& name, const int width,
					 const int height, const int nbBuffers);

			Renderer(const Renderer&)=delete;

			Renderer& operator=(const Renderer&)=delete;

			template<typename Pass, typename... Args>
			void addGeometryPass(const Shader& shader, Args&&... args)
			{
				if(m_geometryPasses == nullptr)
				{
					m_geometryPasses
							= std::make_shared<RenderingNode>(shader);

					m_geometryPasses
							->newPass<Pass>(std::forward<Args>(args)...);
				}
				else
				{
					m_geometryPasses->addPass<Pass>(shader, 
										  std::forward<Args>(args)...);
				}
			}

			template<typename Pass, typename... Args>
			void addPostProcessPass(const Shader& shader, Args&&... args)
			{
				if(m_postProcesses == nullptr)
				{
					m_postProcesses 
							= std::make_shared<RenderingNode>(shader);

					m_postProcesses
							->newPass<Pass>(std::forward<Args>(args)...);
				}
				else
				{
					m_postProcesses->addPass<Pass>(shader, 
										  std::forward<Args>(args)...);
				}
			}

			template<typename Pass>
			std::shared_ptr<Pass> getPass() const
			{
				std::shared_ptr<Pass> res 
							= m_geometryPasses->getPass<Pass>();

				if(res == nullptr)
					res = m_postProcesses->getPass<Pass>();

				if(res == nullptr)
					CIRCE_ERROR("Could not find rendering node");

				return res;
			}


			/** Draw a single model in geometry pass*/
			void draw(Model& model, const Transform transform);

			/** Draw complete graphic pipeline*/
			void draw();

			Camera& getCamera();

			void swapBuffers();
	
			bool isClosed() const;

		private:
			bool m_terminate = false;
			Display m_display;
			bool m_init = false;
			RenderContext m_renderer;
			std::shared_ptr<RenderingNode> m_geometryPasses;
			std::shared_ptr<RenderingNode> m_postProcesses;
			
			void init();
	};
}	
