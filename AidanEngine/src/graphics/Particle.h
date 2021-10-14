#pragma once
#include "../maths/vec2.h"
namespace engine {
	namespace graphics {

		class Particle
		{

		private:
			maths::vec2 m_Position;
			maths::vec2 m_Velocity;
			bool dead;
		public:
			Particle(float xPos, float yPos, float xVel, float yVel);
			void advance();
			void addVelocity(float xAccel, float yAccel);
			bool isDead();
			void draw();
		};

	}
}
