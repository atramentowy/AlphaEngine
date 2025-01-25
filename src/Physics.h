#ifndef PHYSICS_H
#define PHYSICS_H

#include <raylib.h>
#include <btBulletDynamicsCommon.h>

// #include "PhysicsBody.h"

class Physics {
public:
    // Physics config
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* broadphase;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

    // Ground members
    btCollisionShape* groundShape;
    btTransform groundTransform;
    btDefaultMotionState* groundMotionState;
    btRigidBody* groundBody;

    void Init() {
        // Bullet Physics members
        collisionConfig = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfig);
        broadphase = new btDbvtBroadphase();
        solver = new btSequentialImpulseConstraintSolver();
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);

        // Set gravity
        dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

        // Initialize ground (static plane)
        groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0); // Ground is at y=0
        groundTransform.setIdentity();
        groundMotionState = new btDefaultMotionState(groundTransform);

        btRigidBody::btRigidBodyConstructionInfo groundCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
        groundBody = new btRigidBody(groundCI);

        dynamicsWorld->addRigidBody(groundBody);
    }

    void Update() {
        float deltaTime = GetFrameTime();
        float fixedTimeStep = 1.0f / 60.0f; // 60hz physics

        // Handle collision between objA and objB
        int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
        for (int i = 0; i < numManifolds; i++) {
            btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject* objA = contactManifold->getBody0();
            const btCollisionObject* objB = contactManifold->getBody1();
        }
        dynamicsWorld->stepSimulation(deltaTime, 10, fixedTimeStep);
        //dynamicsWorld->stepSimulation(1.0f / 60.0f);
    }

    void Unload() {
        delete dynamicsWorld;
        delete solver;
        delete dispatcher;
        delete collisionConfig;
        delete broadphase;

        // ground
        dynamicsWorld->removeRigidBody(groundBody);
        delete groundBody;
        delete groundMotionState;
        delete groundShape;
    }
};

#endif