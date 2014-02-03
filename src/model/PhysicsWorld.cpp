#include <model/PhysicsWorld.h>



PhysicsWorld::PhysicsWorld()
  : log_("PhysicsWorld"),
    broadphase_(new btDbvtBroadphase()),
    collisionConfiguration_(new btDefaultCollisionConfiguration()),
    dispatcher_(new btCollisionDispatcher(collisionConfiguration_.get())),
    solver_(new btSequentialImpulseConstraintSolver()),
    dynamicsWorld_(new btDiscreteDynamicsWorld(dispatcher_.get(),
                                               broadphase_.get(),
                                               solver_.get(),
                                               collisionConfiguration_.get()))
{
  dynamicsWorld_->setGravity(btVector3(0,-10,0));
}

PhysicsWorld::~PhysicsWorld()
{
}


void PhysicsWorld::stepSimulation(float stepTime)
{
  dynamicsWorld_->stepSimulation(stepTime, 10);
}

