#include "TileLayer.h"
namespace engine {
	namespace graphics {

		TileLayer::TileLayer(Shader* shader)
			: Layer(new BatchRenderer2D(), shader, maths::mat4::orthographic(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f))
		{
			
		}



		TileLayer::~TileLayer()
		{

		}

	
	}
}
