#pragma once
#include "Layer.h"
#include "../BatchRenderer2D.h"
namespace engine {
	namespace graphics {

		class TileLayer : public Layer
		{
		public:
			TileLayer(Shader* shader);
			 ~TileLayer();
		};
	}
}
