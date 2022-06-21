#pragma once

#include <memory>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include "Math.h"
#include "Utils.h"
#include "Event.h"

namespace Circe
{
	struct Intersection
	{
		unsigned int id1;	
		unsigned int id2;	
	};

	template<typename C>
	class PrimitiveVolume
	{
		public:
			virtual bool intersects(const C& other) const = 0;

			void update()
			{
				if(m_id != -1)
					m_callback(m_id);
			}

			void setCallback(const unsigned int id, 
							 const Callback<unsigned int>& callback)
			{
				m_id = id;
				m_callback = callback;
			}

		private:
			unsigned int m_id = -1;
			Callback<unsigned int> m_callback;

	};

	struct GravityWell
	{
		Real mass = (Real)0.0;
		Vec<DIMENSION> position;
		Vec<DIMENSION> velocity;
		Real size2;
		Real d2;
		bool dirty = false;

		void update(const GravityWell& m1, const GravityWell& m2)
		{
			mass = m1.mass + m2.mass;
			
			position = m1.position * m1.mass + m2.position * m2.mass;
			position = position / mass;

			velocity = m1.velocity * m1.mass + m2.velocity * m2.mass;
			velocity = velocity / mass;				
			
			Vec<DIMENSION> dr = m1.position - m2.position;
			size2 = dot(dr,dr);

			dirty = false;
		}

		bool operator<(const GravityWell& other) const
		{
			return mass/d2 > other.mass/other.d2;
		}
	};


	/** N is the dimension, V is inherited from PrimitiveVolume **/
	template<std::size_t N, typename V>
	class BVH
	{
		private:
			struct Node
			{
				V volume;
				unsigned int id;
				GravityWell well;

				std::weak_ptr<Node> parent;
				std::shared_ptr<Node> child1;	
				std::shared_ptr<Node> child2;

				bool isLeaf() const
				{
					return child1 == nullptr;
				}

				bool isRoot() const
				{
					return parent.lock() == nullptr;
				}
			};

			struct NodePair
			{
				NodePair(std::shared_ptr<Node> node1,
						 std::shared_ptr<Node> node2)
						: node1(node1), node2(node2)
				{}

				std::shared_ptr<Node> node1;
				std::shared_ptr<Node> node2;
			};

		public:

			/** 
			  	Insert a node, while choosing the type of volume. The
			    nodes are auto-updated through an observer pattern
			**/
			void insert(const unsigned int id, V& volume)	
			{
				std::shared_ptr<Node> node = std::make_shared<Node>();

				volume.setCallback(id, [this](int id)
				{
					update(id);
				});

				node->volume = volume;
				node->id = id;

				m_leaves.insert(std::pair<unsigned int, 
										  std::shared_ptr<Node>>(id, node));

				m_ids.push_back(id);

				insert(node);
			}
		
			/** 
			  	Insert a node, with the default volume type. The
			    nodes should be updated manually. 
			**/
			void insert(const unsigned int id,
					    const Vec<N>& position, 
					    const Vec<N>& size,
					    const Vec<N>& velocity = Vec<N>(),
					    const Real mass = 0)
			{
				std::shared_ptr<Node> node = std::make_shared<Node>();

				node->volume = V(position, size);
				node->id = id;

				m_leaves.insert(std::pair<unsigned int, 
										  std::shared_ptr<Node>>(id, node));
				m_ids.push_back(id);

				insert(node);

				update(id, position, size, velocity, mass);
			}

			void remove(const unsigned int id)
			{
				if(m_leaves.count(id))
				{
					remove(m_leaves[id]);
					m_leaves.erase(id);
					m_ids.erase(std::remove(m_ids.begin(), m_ids.end(), id),
							m_ids.end());
				}
			}

			void update(const unsigned int id)
			{
				std::shared_ptr<Node> leaf = m_leaves[id];
				std::shared_ptr<Node> parent=leaf->parent.lock();

				if(parent && !leaf->volume.isInside(parent->volume))
				{
					remove(leaf);
					insert(leaf);	
				}
			}
	
			void update(const unsigned int id, 
						const Vec<N>& position,
						const Vec<N>& size,
						const Vec<N>& velocity = Vec<N>(), 
						const Real mass = 0.0)
			{
				std::shared_ptr<Node> leaf = m_leaves[id];
				leaf->volume.setPosition(position);
				leaf->volume.setSize(size);
				leaf->volume.setMargin(velocity);
				leaf->well.mass = mass;
				leaf->well.position = position;
				leaf->well.velocity = velocity;
				leaf->well.dirty = true;
				dirty = true;
				update(id);
			}		

			void updateBH()
			{
				dirty = false;

				std::stack<std::shared_ptr<Node>> stack;

				for(unsigned int id : m_ids)
				{
					if(m_leaves.at(id)->well.dirty)
					{
						stack.push(m_leaves.at(id));
					}
				}
					
				while(!stack.empty())
				{
					std::shared_ptr<Node> node = stack.top();
					stack.pop();

					if(node->isRoot()) continue;

					if(std::shared_ptr<Node> parent = node->parent.lock())
					{
						std::shared_ptr<Node> c1 = parent->child1;
						std::shared_ptr<Node> c2 = parent->child2;

						if(c1->well.dirty || c2->well.dirty)
						{
							parent->well.update(c1->well, c2->well);

							stack.push(parent);

							parent->well.dirty = true;
							c1->well.dirty = false;
							c2->well.dirty = false;
						}
					}
				}
			}

			std::vector<Intersection> getInternalContacts()
			{
				std::vector<Intersection> res;
				std::stack<NodePair> stack;

				if(!m_root || m_root->isLeaf()) return res;

				stack.push(NodePair(m_root->child1, m_root->child2));

				while(!stack.empty())
				{
					NodePair pair = stack.top();
					stack.pop();

					std::shared_ptr<Node> node1 = pair.node1;
					std::shared_ptr<Node> node2 = pair.node2;

					if(!node1->volume.intersects(node2->volume))
						continue;

					if(node1->isLeaf() && node2->isLeaf())
					{
						Intersection contact;
						contact.id1 = node1->id;
						contact.id2 = node2->id;
						res.push_back(contact);
						continue;
					}

					if(node2->isLeaf() || 
					  (!node1->isLeaf() && node1->volume.getArea() 
										  >= node2->volume.getArea()))
					{
						stack.push(NodePair(node1->child1, node2));
						stack.push(NodePair(node1->child2, node2));
					}
					else
					{
						stack.push(NodePair(node1, node2->child1));
						stack.push(NodePair(node1, node2->child2));
					}
				}

				return res;
			}

			/** 
				Get the list of id which intersect the chosen volume.
			**/
			std::vector<unsigned int> query
				(const std::shared_ptr<PrimitiveVolume<V>> volume)
			{
				std::vector<unsigned int> result;
				std::stack<std::shared_ptr<Node>> stack;
				stack.push(m_root);

				while(!stack.empty())
				{
					std::shared_ptr<Node> node = stack.top();

					if(volume->intersects(node->volume))
					{
						if(node->isLeaf())
						{
							result.push_back(node->id);
						}
						else
						{
							stack.push(node->child1);
							stack.push(node->child2);
						}
					}

					stack.pop();
				}

				return result;
		    }


			/** 
			  	Barnes Hut gravity: theta goes from 1.5 (inaccurate)
			    to 0.0 (n^2)
			 **/
			std::set<GravityWell> getGravityWells(const unsigned int id, 
						   	  		   		 const Real theta = 1.0)
			{
				if(dirty)
				{
					updateBH();
				}

				std::set<GravityWell> wells;

				Vec<N> position = m_leaves[id]->well.position;

				std::stack<std::shared_ptr<Node>> stack;
				stack.push(m_root);
			
				Real ratio;
				while(!stack.empty())
				{
					std::shared_ptr<Node> node = stack.top();
					stack.pop();

					if(node->well.mass == 0.0) continue;
					if(node->id == id) continue;

					Vec<N> dr = node->well.position - position;
					Real d2 = dot(dr, dr);

					node->well.d2 = d2;

					if(node->isLeaf())
					{
						wells.insert(node->well);
					}
					else
					{
						ratio = std::sqrt(node->well.size2/d2);

						if(ratio < theta 
						   && !m_leaves[id]->volume.isInside(node->volume))
						{
							wells.insert(node->well);
						}
						else
						{
							stack.push(node->child1);	
							stack.push(node->child2);	
						}
					}
					

				}

				return wells;
			}

			typename std::vector<unsigned int>::iterator begin()
			{
				return m_ids.begin();
			}

			typename std::vector<unsigned int>::iterator end()
			{
				return m_ids.end();
			}

			void draw()
			{
				if(m_root) draw(m_root);
			}

			void draw(const std::shared_ptr<Node> node)
			{
				Vec3 color(0,0,1);
				if(node->isLeaf()) color = Vec3(1,0,0);
				node->volume.draw(color);
				if(!node->isLeaf())
				{
					draw(node->child1);
					draw(node->child2);
				}
			}

		private:
			std::shared_ptr<Node> m_root;
			std::map<unsigned int, std::shared_ptr<Node>> m_leaves;
			std::vector<unsigned int> m_ids;
			bool dirty = false;

			void insert(const std::shared_ptr<Node> leaf)
			{
				/** If the tree is empty */
				if(m_root == nullptr)
				{
					m_root = leaf;
					return;
				}

				std::shared_ptr<Node> sibling = findBest(leaf->volume);

				/** Attach the node */
				std::shared_ptr<Node> parent = std::make_shared<Node>();

				leaf->parent = parent;
				parent->child1 = leaf;

				if(!sibling->isRoot())
				{
					std::shared_ptr<Node> grandParent = 
												sibling->parent.lock();

					if(grandParent->child1 == sibling)
					{
						grandParent->child1 = parent;
					}
					else
					{
						grandParent->child2 = parent;
					}
					parent->parent = grandParent;
				}
				else
				{
					m_root = parent;
				}

				sibling->parent = parent;
				parent->child2 = sibling;
				
				/** Refit the tree  */
				refit(parent);
			}

			void remove(std::shared_ptr<Node> leaf)
			{
				if(leaf->isRoot())
				{
					m_root.reset();
					return;
				}

				std::shared_ptr<Node> parent = leaf->parent.lock();
				std::shared_ptr<Node> sibling;

				if(parent->child1 == leaf)
				{
					sibling = parent->child2;
				}
				else
				{
					sibling = parent->child1;
				}

				std::shared_ptr<Node> grandParent = parent->parent.lock();

				/** If the parent was the root **/
				if(grandParent == nullptr)
				{
					m_root = sibling;
					sibling->parent.reset();
					parent.reset();
				}
				else
				{
					if(grandParent->child1 == parent)
					{
						grandParent->child1 = sibling;
					}
					else
					{
						grandParent->child2 = sibling;
					}
					sibling->parent = grandParent;
					parent.reset();

					refit(grandParent);	
				}
			}


			std::shared_ptr<Node> findBest(const V& newVolume) const
			{
				std::shared_ptr<Node> node = m_root;

				while(!node->isLeaf())
				{

					Real cost = newVolume.getUnionArea(node->volume);
					Real upstreamCost = cost - node->volume.getArea();	
				
					std::shared_ptr<Node> child1 = node->child1;
					Real cost1 = upstreamCost 
							   + newVolume.getUnionArea(child1->volume);

					if(!child1->isLeaf())
						cost1 -= child1->volume.getArea();

					std::shared_ptr<Node> child2 = node->child2;
					Real cost2 = upstreamCost 
							   + newVolume.getUnionArea(child2->volume);

					if(!child2->isLeaf())
						cost2 -= child2->volume.getArea();

					if(cost < std::min(cost1, cost2)) break;

					if(cost1 < cost2)
					{
						node = child1;
					}
					else
					{
						node = child2;
					}
				}

				return node;
			}

			/**
			 	Backpropagate the change in shape or position
			    of a node.	
			**/
			void refit(const std::shared_ptr<Node> startingNode)
			{
				std::shared_ptr<Node> parent = startingNode;
				while(parent != nullptr)
				{
					std::shared_ptr<Node> child1 = parent->child1;
					std::shared_ptr<Node> child2 = parent->child2;

					parent->volume.refit(child1->volume, child2->volume);

					parent = parent->parent.lock();
				}
			}
	};
}
