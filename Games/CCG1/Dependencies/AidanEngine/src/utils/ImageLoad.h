#pragma once
#include "../../vendor/stb_image/stb_image.h"
namespace engine {
	namespace graphics {

		static unsigned char* load_image(const char* filename, GLsizei* width, GLsizei* height, int* bPP)
		{
			stbi_set_flip_vertically_on_load(1);

			unsigned char* bits = stbi_load(filename, width, height, bPP, 4);
			return bits;
		}
	}
}