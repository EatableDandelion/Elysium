#pragma once

#include <memory>
#include <vector>
#include <stack>
#include "Math.h"

namespace Circe
{
	struct BroadContact
	{
		unsigned int id1;	
		unsigned int id2;	
	};

	template<typename C>
	struct PrimitiveCollider
	{
		virtual bool intersects(const C& other) const = 0;
	};

	template<std::size_t DIMENSION, typename V>
	class BVH
	{
		private:
			struct Node
			{
				V volume;
				unsigned int id;
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
			std::shared_ptr<Node> insert(const unsigned int id, 
										 const Vec<DIMENSION>& position, 
										 const Vec<DIMENSION>& size,
										 const Real mass = 0)
			{
				std::shared_ptr<Node> node = std::make_shared<Node>();
				node->volume = V(position, size);
				node->id = id;

				m_leaves.insert
				(std::pair<unsigned int, std::shared_ptr<Node>>(id, node));

				insert(node);

				return node;
			}
		
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

			void remove(const unsigned int id)
			{
				remove(m_leaves[id]);
				m_leaves.erase(id);
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

			void update(const unsigned int id, 
						const Vec<DIMENSION>& position,
						const Vec<DIMENSION>& velocity = Vec<DIMENSION>(), 
						const Real mass = 0.0)
			{
				std::shared_ptr<Node> leaf = m_leaves[id];
				leaf->volume.setPosition(position);
				leaf->volume.setMargin(velocity);
				leaf->volume.setMass(mass);

				std::shared_ptr<Node> parent=leaf->parent.lock();

				if(parent && !leaf->volume.isInside(parent->volume))
				{
					remove(leaf);
					insert(leaf);	
				}
			}
			
			std::vector<BroadContact> findInternalContacts()
			{
				std::vector<BroadContact> res;
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
						BroadContact contact;
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

			std::vector<unsigned int> query
				(const std::shared_ptr<PrimitiveCollider<V>> volume)
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
			Vec<DIMENSION> getGravity(const unsigned int id, 
						   const Real theta = 1.0)
			{
				Vec<DIMENSION> position = m_leaves[id]->volume.getCoG();

				std::stack<std::shared_ptr<Node>> stack;
				stack.push(m_root);
			
				Vec<DIMENSION> force;

				Real ratio;
				Vec<DIMENSION> dr;
				Real mass;
				while(!stack.empty())
				{
					std::shared_ptr<Node> node = stack.top();

					if(node->volume.mass > 0.0)
					{
						dr = node->volume.getCoG() - position;
				      	if(node.isLeaf())
						{
							if(node->id != id)
							{
								force = force + node->volume.getMass()
											  * normalize(dr)/dot(dr, dr);
							}
							else
							{
								mass = node->volume.getMass();
							}
						}
						else
						{
							ratio=node->volume.getBarnesHutRatio(dr);

							if(ratio < theta)
							{

								force = force + node->volume.getMass()
										  	  * normalize(dr)/dot(dr, dr);

							}
							else
							{
								stack.push(node->child1);	
								stack.push(node->child2);	
							}
						}
					}

					stack.pop();
				}

				return force * mass;
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


/*	struct TestBody
	{};

	template<typename Body>
	struct Contact
	{
		std::shared_ptr<Body> bodies[2];
	};

	template<typename Volume, typename Body>
	class BVHNode : public 
						std::enable_shared_from_this<BVHNode<Volume,Body>>
	{
		public:
			BVHNode(const Volume& volume, const std::shared_ptr<Body> body,
					const std::shared_ptr<BVHNode<Volume, Body>> parent)
			   :m_parent(parent), m_volume(volume), m_body(body)
			{}	

			~BVHNode()
			{
				remove();
			}

			bool isLeaf() const
			{
				return m_body != nullptr;
			}

			unsigned int getContacts(std::vector<Contact<Body>>& contacts, 
									 unsigned int limit) const
			{
				if(isLeaf() || limit == 0) return 0;

				return m_children[0]->getContactsWith(m_children[1], 
													  contacts, limit);
			}

			unsigned int getContactsWith(const BVHNode<Volume, Body>& other,
									 std::vector<Contact<Body>>& contacts,
									 unsigned int limit) const
			{
				if(!overlaps(other) || limit == 0) return 0;

				if(isLeaf() && other.isLeaf())
				{
					Contact<Body> contact;
					contact.bodies[0] = m_body;
					contact.bodies[1] = other.m_body;
					contacts.push_back(contact);

					return 1;
				}

				if(other.isLeaf() || (!isLeaf() && m_volume.getArea() 
									  >= other.m_volume.getArea()))
				{
					unsigned int count = 
					 m_children[0]->getContactsWith(other,contacts,limit);

					if(limit > count) 
					{
						return count + 
							 m_children[1]->getContactsWith(other, contacts,
									   						limit - count);
					}
					else
					{
						return count;
					}
				}
				else
				{
					unsigned int count = getContactsWith(
							other.m_children[0], contacts, limit);

					if(limit > count)
					{
						return count + getContactsWith(
							other.m_children[1], contacts, limit - count);
					}
					else
					{
						return count;
					}
				}
			}

			bool overlaps(const BVHNode<Volume, Body>& other) const
			{
				return m_volume.overlaps(other.m_volume);	
			}

			void insert(std::shared_ptr<Body> body, const Volume& volume)
			{
				if(isLeaf())
				{
					m_children[0] = std::make_shared<BVHNode<Volume, Body>>
							(m_volume, m_body, this->shared_from_this());

					m_children[1] = std::make_shared<BVHNode<Volume, Body>>
							(volume, body, this->shared_from_this());

					m_body.reset();
					m_volume.recalculate(m_volume, volume);
				}
				else
				{
					Real cost0 = m_children[0]->m_volume.getGrowth(volume);
					Real cost1 = m_children[1]->m_volume.getGrowth(volume);

					if(cost0 < cost1)
					{
						m_children[0]->insert(body, volume);
					}
					else
					{
						m_children[1]->insert(body, volume);
					}
				}
			}

			void remove()
			{
				std::shared_ptr<BVHNode<Volume, Body>> parent = 
						m_parent.lock();
				if(parent)
				{
					std::shared_ptr<BVHNode<Volume, Body>> sibling;

					if(this->shared_from_this() == parent->m_children[0])
						sibling = parent->m_children[1];
					else
						sibling = parent->m_children[0];

					parent->m_volume = sibling->m_volume;
					parent->m_body = sibling->m_body;
					parent->m_children[0] = sibling->m_children[0];
					parent->m_children[1] = sibling->m_children[1];

					sibling->m_parent.reset();
					sibling->m_children[0].reset();
					sibling->m_children[1].reset();
				
					parent->recalculate();
				}

				if(m_children[0])
				{
					m_children[0]->m_parent.reset();
				}
				if(m_children[1])
				{
					m_children[1]->m_parent.reset();
				}
			}

			void recalculate()
			{
				if(!isLeaf())
				{
					m_volume.recalculate(m_children[0]->m_volume,
										 m_children[1]->m_volume);
				}
			}

			void getAllVolumes(std::vector<Volume>& volumes) 
			{
				int i = 0;
				if(isLeaf()) i = 1;
				m_volume.type = i;
				volumes.push_back(m_volume);
				if(!isLeaf())
				{
					m_children[0]->getAllVolumes(volumes);
					m_children[1]->getAllVolumes(volumes);
				}
			}

		private:
			Volume m_volume;
			std::shared_ptr<BVHNode<Volume, Body>> m_children[2];
			std::shared_ptr<Body> m_body;
			std::weak_ptr<BVHNode<Volume, Body>> m_parent;
	};


	template<std::size_t N>
	class BoundingSphere
	{
		public:
			int type;

			BoundingSphere()
			{}

			BoundingSphere(const Vec<N>& center, const Real& radius)
				: m_center(center), m_radius(radius)
			{}

			BoundingSphere(const BoundingSphere<N>& other)
				: m_center(other.m_center), m_radius(other.m_radius)
			{}

			BoundingSphere(const BoundingSphere<N>& sphere1,
						   const BoundingSphere<N>& sphere2)
			{
				recalculate(sphere1, sphere2);
			}

			void recalculate(const BoundingSphere<N>& sphere1,
						     const BoundingSphere<N>& sphere2)
			{
				Vec<N> centerOffset = sphere2.m_center - sphere1.m_center;

				Real d = dot(centerOffset, centerOffset);

				Real dr = sphere2.m_radius - sphere1.m_radius;

				if(dr*dr >= d)
				{
					if(sphere1.m_radius > sphere2.m_radius)
					{
						m_center = sphere1.m_center;
						m_radius = sphere1.m_radius;					
					}
					else
					{
						m_center = sphere2.m_center;
						m_radius = sphere2.m_radius;
					}	
				}
				else
				{
					d = std::sqrt(d);
					Real r = (d+sphere1.m_radius+sphere2.m_radius)*0.5;
					m_center = sphere1.m_center;
					if(d > 0)
					{
						m_center = m_center + centerOffset
									*((r - sphere1.m_radius)/d);
					}	
					m_radius = r;
				}
			}

			bool overlaps(const BoundingSphere<N>& other) const
			{
				Vec<N> diff = (m_center - other.m_center);
				Real d2 = dot(diff, diff);
				return d2 < (m_radius+other.m_radius)
						   *(m_radius+other.m_radius);
			}

			Real getArea() const
			{
				return m_radius*m_radius*3.14159;
			}

			Real getGrowth(const BoundingSphere<N>& other)
			{
				Vec<N> centerOffset = other.m_center - m_center;

				Real d = std::sqrt(dot(centerOffset, centerOffset));

				Real r = (d+m_radius+other.m_radius)*0.5;

				return r*r*3.14159;
			}

			Vec<N> getPosition() const
			{
				return m_center;
			}

			void setPosition(const Vec<N>& center)
			{
				m_center = center;
			}

			Real getRadius() const
			{
				return m_radius;
			}

			void setRadius(const Real radius)
			{
				m_radius = radius;
			}

		private:
			Vec<N> m_center;
			Real m_radius; 
	};*/
}
