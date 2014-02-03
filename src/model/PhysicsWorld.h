#ifndef MODEL_PHYSICSWORLD_H
#define MODEL_PHYSICSWORLD_H

#include <memory>

#include <btBulletDynamicsCommon.h>
#include <util/Log.h>

/**
 * PhysicsWorld class.
 *
 * // TODO swarminglogic, 2014-01-18: Remove Class? Refactor?
 * Simply for testing purposes.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class PhysicsWorld
{
public:
  PhysicsWorld();
  ~PhysicsWorld();

  void stepSimulation(float stepTime);

private:
  Log log_;

  // NonCopyable
  PhysicsWorld(const PhysicsWorld& c);
  PhysicsWorld& operator=(const PhysicsWorld& c);

  std::unique_ptr<btBroadphaseInterface> broadphase_;
  std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration_;
  std::unique_ptr<btCollisionDispatcher> dispatcher_;
  std::unique_ptr<btSequentialImpulseConstraintSolver> solver_;
  std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld_;
};

#endif
