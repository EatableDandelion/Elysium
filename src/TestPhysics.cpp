#include "TestPhysics.h"

void TestPhysics::init(std::shared_ptr<Context> context)
{
	using namespace Elysium;

	context->setRenderer(std::make_shared<Renderer>("Elysium",800,600,3));

	context->setShadersDirectory("../res/shaders/");
	context->setTexturesDirectory("../res/textures/");
	context->setModelsDirectory("../res/models/");

	std::shared_ptr<Physics::PhysicsEngine> phi = 
							std::make_shared<Physics::PhysicsEngine>();
	
	context->setPhysicsEngine(phi);



	context->getRenderer()->addGeometryPass<GeometryPass>
						   (context->newShader("geometry"));

	context->getRenderer()->addPostProcessPass<DirectionalLightPass>(
							context->newShader("directionalLight"), 
					 		context->newMesh("sprite"));

	context->getRenderer()
			->addPostProcessPass<AmbientPass>(context->newShader("screen"),
								   context->newMesh("sprite"), 0.0f);

	context->getRenderer()->addPostProcessPass<DebugPass>
							(context->newShader("debug"));

	Debug.setDebugRenderer(context->getRenderer()->getPass<DebugPass>());

	context->getRenderer()
		   ->getPass<DirectionalLightPass>()
		   ->addLight(DirectionalLight(Vec3(0.5,0,-1), 
									   Vec3(1.0,1.0,1.0), 1.0f));

	context->getRenderer()->getCamera().getTransform()
						  ->translate(Vec3(0,0,-10));

	sprite1 = context->newEntity();
	sprite1->addComponent<RenderingComponent>(
						Model(context->newMesh("sprite"), 
							  context->newTexture("bricks2.png")));

	sprite1->getTransform()->translate(Vec(1.5,7.7));

	std::vector<Vec> 
			collisionPoints({Vec(-1,-1),Vec(-1,1),Vec(1,1),Vec(1,-1)});
	
	sprite1->addComponent<Physics::PhysicsComponent>(10.0, 
						   sprite1->getTransform(), collisionPoints);

	sprite1->getComponent<Physics::PhysicsComponent>()
	  ->addForceGenerator(std::make_shared<Physics::GravityGenerator>(10));

	sprite2 = context->newEntity();
	sprite2->addComponent<RenderingComponent>(
						Model(context->newMesh("sprite"), 
							  context->newTexture("bricks2.png")));

	sprite2->addComponent<Physics::PhysicsComponent>(100.0, 
							   sprite2->getTransform(), collisionPoints);

}

/*
	//Create an entity from model:
   	Entity cyborg = newEntity();
	cyborg->addComponent<RenderingComponent>(
							context.newModel("cyborg/cyborg.obj"));
*/

/*
	//Set-up inputs:

	mover = std::make_shared<Mover>(elysium.getCamera().getTransform());

	getInput()->getKeyListener(Input::KEY_W)
						.addListener(mover->getForwardListener());
	getInput()->getKeyListener(Input::KEY_A)
						.addListener(mover->getLeftListener());
	getInput()->getKeyListener(Input::KEY_S)
						.addListener(mover->getBackwardListener());
	getInput()->getKeyListener(Input::KEY_D)
						.addListener(mover->getRightListener());
	getInput()->getMouseMotion()
						.addListener(mover->getRotateListener());
*/
/*
   	//Push entity
	sprite1->getComponent<Physics::PhysicsComponent>()->addForce(Vec(0,-50.0));
	sprite1->getComponent<Physics::PhysicsComponent>()->addTorque(50.0);
*/
/*
	//Add a force to an entity
    sprite1->getComponent<PhysicsComponent>()
		  ->addForceGenerator(std::make_shared<SpringGenerator>(2, 0.1, 5,
					  									   Vec2(0.0, 0.0)));
*/
/*
	//Add a constraint:
	std::shared_ptr<Physics::PhysicsEngine> phi = std::make_shared<Physics::PhysicsEngine>();
	
	std::shared_ptr<Physics::Joint> joint = 
		std::make_shared<Physics::Slider>(
					  sprite1->getComponent<Physics::PhysicsComponent>(),
					  sprite2->getComponent<Physics::PhysicsComponent>());

	phi->addConstraint(joint);

*/
/*
	//Add debug draw
    Debug.drawLine(sprite1->getTransform()->getPosition(),
				   sprite2->getTransform()->getPosition());
*/


/*
void BVH::insert(const int objectIndex, const Ball& volume)
{
	int leafIndex = createLeafNode(objectIndex, volume);
	
	if(m_nodes.size() == 1)
	{
		m_root = leafIndex;
		return;
	}
	
	//find best sibling
	int sibling = 0;
   	//for(int i = 0; i<nodeCount; i++)
   	{
 	 // 	sibling = pickBest(sibling, i);
   	}	   

	//create new parent
	int oldParent = m_nodes[sibling].parentIndex;
	int newParent = createInternalNode();
	m_nodes[newParent].parentIndex = oldParent;
	Ball parentVolume(volume, m_nodes[sibling].volume);
	m_nodes[newParent].volume = parentVolume;
	
	if(oldParent != -1)
	{
		if(m_nodes[oldParent].child1 == sibling)
		{
			m_nodes[oldParent].child1 = newParent;
		}
		else
		{
			m_nodes[oldParent].child2 = newParent;
		}

		m_nodes[newParent].child1 = sibling;
		m_nodes[newParent].child2 = leafIndex;
		m_nodes[sibling].parentIndex = newParent;
		m_nodes[leafIndex].parentIndex = newParent;
	}
	else
	{
		m_nodes[newParent].child1 = sibling;
		m_nodes[newParent].child2 = leafIndex;
		m_nodes[sibling].parentIndex = newParent;
		m_nodes[leafIndex].parentIndex = newParent;
		m_root = newParent;
	}

	//refit parents
	refitAncestors(leafIndex);	
}
*/
/** TODO **/
/*int BVH::pickBest(const int bestSibling, const int nodeIndex)
{
	return 0;
}

int BVH::newIndex()
{
	if(m_freeNodes.empty())
	{
		for(int i = 0; i<10; i++)
			m_freeNodes.push(++m_maxIndex);
	}

	int index = m_freeNodes.top();
	m_freeNodes.pop();

	return index;
}

void BVH::deleteNode(const int index)
{
	m_freeNodes.push(index);
	m_nodes.erase(index);
}

void BVH::refitAncestors(const int startIndex)
{
	int index = m_nodes[startIndex].parentIndex;
	while(index != NULL_INDEX)
	{
		int child1 = m_nodes[index].child1;
		int child2 = m_nodes[index].child2;

		Ball newVolume(m_nodes[child1].volume, m_nodes[child2].volume);
		m_nodes[index].volume = newVolume;

		index = m_nodes[index].parentIndex;
	}
}

int BVH::createLeafNode(const int objectIndex, const Ball& volume)
{
	int index = createInternalNode();
	Node node = m_nodes[index];
	node.volume = volume;
	node.objectIndex = objectIndex;

	return index;
}

int BVH::createInternalNode()
{
	Node node;
	node.volume = volume;
	node.objectIndex = objectIndex;
	node.parentIndex = NULL_INDEX;
	node.child1 = NULL_INDEX;
	node.child2 = NULL_INDEX;

	int index = newIndex();
	m_nodes.insert(std::pair<int, Node>(index, node));

	return index;
}

void BVH::removeLeaf(const int index)
{
	if(index == m_root)
	{
		deleteNode(m_root);
		m_root = NULL_INDEX;
		return;
	}

	Node node = m_nodes[index];

	int parent = node.parentIndex;
	int grandParent = NULL_INDEX;
	
	grandParent = m_nodes[parent].parentIndex;

	int sibling = m_nodes[parent].child1;
	if(sibling == index)
		sibling = m_nodes[parent].child2;

	if(grandParent != NULL_INDEX)
	{
		if(m_nodes[grandParent].child1 == parent)
		{
			m_nodes[grandParent].child1 = sibling;
		}
		else
		{
			m_nodes[grandParent].child2 = sibling;
		}
		m_nodes[sibling].parentIndex = grandParent;

		deleteNode(parent);
	
		refitAncestors(grandParent);	
	}
	else
	{
		m_root = sibling;
		m_nodes[sibling].parentIndex = NULL_INDEX;

		deleteNode(parent);
	}

	deleteNode(index);
}
*/
/*Real BVH::getCost() const
{
	Real cost = 0.0;

	for(int i = 0; i<nodeCount; i++)
	{
		if(!nodes[i].isLeaf)
		{
			cost += nodes[i].volume.getArea();
		}
	}

	return cost;
}*/


