#pragma once
#include "Renderable2D.h"
namespace engine {
	namespace graphics {


		class Sprite : public Renderable2D
		{
		private:

		public:
			Sprite(float x, float y, float width, float height, maths::vec4& color);
			Sprite(float x, float y, float width, float height, Texture* texture);
		};
	}
}
