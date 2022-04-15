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
#include "Game/Event.h"

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
				if(m_next == nullptr) 
					CIRCE_ERROR("Could not find rendering node");

				if(m_next->getID() == std::type_index(typeid(Pass)))
				{
					return std::dynamic_pointer_cast<Pass>(m_next->m_pass);
				}
				return m_next->getPass<Pass>();
			}

			/** Init this pass, calls child pass */
			virtual void init(RenderContext& renderer);

			/** Draw this pass, calls child pass */
			virtual void draw(RenderContext& renderer);

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

	class Renderer : public RenderingNode 
	{
		public:
			Renderer(const std::string& name, const int width,
					 const int height, const int nbBuffers);

			Renderer(const Renderer&)=delete;

			Renderer& operator=(const Renderer&)=delete;

			template<typename Pass, typename... Args>
			void setFirstPass(const Shader& shader, Args&&... args)
			{
				m_shader = shader;
				newPass<Pass>(std::forward<Args>(args)...);
			}

			void init();

			/** Draw a single model in geometry pass*/
			void draw(Model& model, const Transform transform);

			/** Draw complete graphic pipeline*/
			void draw();

			Camera& getCamera();

			void swapBuffers();
	
			bool isClosed() const;

		private:
			bool m_terminate = false;
			bool m_wasInit = false;
			bool m_shaderBound = false;
			Display m_display;
			RenderContext m_renderer;
	};
}	
