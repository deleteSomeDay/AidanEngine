#pragma once
#include "Renderable2D.h"
namespace engine {
	namespace graphics {

		class Label : public Renderable2D
		{
		private:
			maths::vec3& position;
			std::string text;
			float m_Scale;
		public:
			Label(std::string text, int x, int y, maths::vec4 color, float scale);

			virtual void submit(Renderer2D* renderer) const override;
		};
	}
}
