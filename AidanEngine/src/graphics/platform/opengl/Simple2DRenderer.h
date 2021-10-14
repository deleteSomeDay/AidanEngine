#pragma once

#include <deque>
#include <GL/glew.h>
#include "../maths/maths.h"
#include "Renderer2D.h"
#include "Static_Sprite.h"
namespace engine {
	namespace graphics {

		class Simple2DRenderer : public Renderer2D
		{
		private:
			std::deque<const StaticSprite*> m_RenderQueue;
		public:
			virtual void submit(const Renderable2D* renderable) override;
			virtual void flush() override;
		};
	}
}
