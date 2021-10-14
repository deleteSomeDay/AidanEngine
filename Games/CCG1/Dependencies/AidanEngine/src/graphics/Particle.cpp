#include "Particle.h"

namespace engine {
	namespace graphics {

		Particle::Particle(float xPos, float yPos, float xVel, float yVel)
			: m_Position(maths::vec2(xPos,yPos)), m_Velocity(maths::vec2(xVel,yVel))
		{

		}
		void Particle::advance()
		{

		}
		void Particle::addVelocity(float xAccel, float yAccel)
		{

		}
		bool Particle::isDead()
		{
			return false;
		}
		void Particle::draw()
		{

		}

	}
}

