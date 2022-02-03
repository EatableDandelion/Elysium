#pragma once

#include <vector>
#include <memory>
#include <Circe/Circe.h>
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "RenderingPass.h"

namespace Elysium
{
	typedef unsigned int PassID;

	class RenderingNode
	{
		public:
			RenderingNode(const Shader& shader, const PassID id);

			/** Init this pass, calls child pass */
			void init(Renderer& renderer);

			/** Draw this pass, calls child pass */
			void draw(Renderer& renderer);

			/** Add a model to render */
			void addModel(const Model& model);

			/** Add the pass rendered after this one */
			template<typename Pass>
			PassID addPass(const Shader& shader)
			{
				if(m_next == nullptr)
				{
					m_next = std::make_shared<RenderingNode>(shader,m_id+1);
					m_next->newPass<Pass>();

					return m_next->getID();
				}
				else
				{
					return m_next->addPass<Pass>(shader);
				}
			}

			template<typename Pass>
			void newPass()
			{
				m_pass = std::make_shared<Pass>();
			}

			PassID getID() const;

			std::shared_ptr<RenderingNode> getRenderingNode
												(const PassID id) const;

		private:
			PassID m_id;
			Shader m_shader;
			std::vector<Model> m_models;
			std::shared_ptr<RenderingNode> m_next;
			std::shared_ptr<RenderingPass> m_pass;
	};

	class RenderingEngine
	{
		public:
			RenderingEngine(const std::string& name, const int width,
							const int height, const int nbBuffers);

			void init();

			void draw();

			template<typename Pass>
			PassID addPass(const Shader& shader)
			{
				if(m_firstNode == nullptr)
				{
					m_firstNode = std::make_shared<RenderingNode>(shader,0);
					m_firstNode->newPass<Pass>();

					return m_firstNode->getID();
				}
				else
				{
					return m_firstNode->addPass<Pass>(shader);
				}
			}

			std::shared_ptr<RenderingNode> operator()(const PassID id) const;
	
			Camera& getCamera();

			void swapBuffers();
	
			bool isClosed() const;
	
		private:
			bool m_wasInit = false;
			Display m_display;
			Renderer m_renderer;
			std::shared_ptr<RenderingNode> m_firstNode;
	};

}	
