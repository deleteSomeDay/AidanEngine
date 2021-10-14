
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "OpenGL.h"
#include "std.h"

namespace engine
{

	void OpenGL::initGL()
	{
		if (glewInit())
		{
			std::cout << "error: GLEWINIT" << std::endl;
		}
		assert(glewInit());

	}

}