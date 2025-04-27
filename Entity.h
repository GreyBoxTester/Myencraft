#pragma once
#include "AABB.h"
#include "World.h"
#include "Position.h"

namespace mc
{
	class Entity
	{
	public:
		Entity(const mc::Position& pos, const cl::Vector3f& colliderSize);
		void update(const World& world, float dt);
		void addAcceleration(const cl::Vector3f& acceleration);
		void setColliderSize(const cl::Vector3f& colliderSize);
		void setPosition(const cl::Vector3f& position);
		const cl::Vector2f& getRotation() const;
		const Position& getPosition() const;
	private:
		float collide(const World& world, float time, float dt);
		void calculateEntryExit(const AABB& block, const cl::Vector3f& velocity, cl::Vector3f* distEntryOut, cl::Vector3f* distExitOut, cl::Vector3f* entryOut, cl::Vector3f* exitOut);
		void calculateHitNormal(const cl::Vector3f& entry, const cl::Vector3f& distEntry, cl::Vector3f* normalOut);
	protected:
		Position pos;
		cl::Vector3f vel;
		cl::Vector2f rot;
		cl::Vector3f accel;
		AABB collider;
		cl::Vector3f centerOffset;
		bool onGround;
		bool inWater;
	};
}
