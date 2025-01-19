#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include "Physics.h"

// #include <raylib.h>
// #include <btBulletDynamicsCommon.h>

class PhysicsBody {
public:
    btRigidBody* body; // Pointer to the Bullet rigid body
    btCollisionShape* shape;

    PhysicsBody(btDiscreteDynamicsWorld* world, btCollisionShape* bodyShape, const btVector3& startPosition, float mass) {
        // Set object shape
        shape = bodyShape;

        // Create transform
        btTransform transform;
        transform.setIdentity();
        transform.setOrigin(startPosition);

        // Calculate inertia (for dynamic bodies)
        btVector3 localInertia(0, 0, 0);
        if (mass != 0.0f) { // Calculate only for dynamic objects
            shape->calculateLocalInertia(mass, localInertia);
        }
        
        // Create motion state
        btDefaultMotionState* motionState = new btDefaultMotionState(transform);

        // Create rigidbody construction info
        btRigidBody::btRigidBodyConstructionInfo rigidbodyCI(mass, motionState, shape, localInertia);

        // Create rigidbody
        body = new btRigidBody(rigidbodyCI);

        // Add the body to the dynamics world
        world->addRigidBody(body);
        body->setGravity(world->getGravity()); // set body gravity
    }

    ~PhysicsBody() {
        // Clean up Bullet physics memory
        delete body->getMotionState();
        delete body;  // RigidBody is responsible for deleting its own shape if needed
    }

    // Apply a force to the body
    void ApplyForce(const btVector3& force) {
        body->applyCentralForce(force);
    }

    // Set velocity
    void SetVelocity(const btVector3& velocity) {
        body->setLinearVelocity(velocity);
    }

    void ApplyMovement(const Vector3& movement, float speed) {
        btVector3 velocity = btVector3(movement.x, 0.0f, movement.z) * speed;  // Ignoring vertical movement
        // Apply the velocity to the physics body
        body->setLinearVelocity(velocity);

        // Get the current velocity (if any)
        btVector3 currentVelocity = body->getLinearVelocity();

        // Convert movement to velocity vector
        btVector3 movementVelocity = btVector3(movement.x, 0.0f, movement.z) * speed;

        // Get the world gravity
        //btVector3 gravity = world->getGravity();
        btVector3 gravity = body->getGravity();
        //btVector3 gravity = btVector3(0, -9.81f, 0);

        // Combine gravity and movement velocity
        btVector3 newVelocity = movementVelocity + gravity;

        // Set the new velocity
        body->setLinearVelocity(newVelocity);
    }

    // Get position
    Vector3 GetPosition() const {
        btTransform transform;
        body->getMotionState()->getWorldTransform(transform);
        btVector3 btVec = transform.getOrigin();
        return Vector3{ btVec.x(), btVec.y(), btVec.z() };
    }
};

#endif