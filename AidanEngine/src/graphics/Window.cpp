#include "window.h"

namespace engine {
	namespace graphics {
		void window_resize(GLFWwindow *window, int width, int height);
		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

		Window::Window(const char *name, int width, int height)
		{
			m_Title = name;
			m_Width = width;
			m_Height = height;
			init();

			for (int i = 0; i < MAX_KEYS; i++)
			{
				m_Keys[i] = false;
			}
			for (int i = 0; i < MAX_BUTTONS; i++)
			{
				m_MouseButtons[i] = false;
			}
		}

		Window::~Window()
		{
			glfwTerminate();
		}

		void Window::update()
		{
			glfwPollEvents();
			glViewport(0, 0, m_Width, m_Height);
			glfwSwapBuffers(m_Window);
		}
		void Window::Clear() const
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}
		bool Window::closed() const
		{
			return glfwWindowShouldClose(m_Window) == 1;
		}

		bool Window::init()
		{
			if (!glfwInit()) //init glfw
			{
				exit(EXIT_FAILURE);
				return false;
			}
			m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, m_Monitor, NULL);
			if (!m_Window)
			{
				return false;
				exit(EXIT_FAILURE);
			}
			glfwMakeContextCurrent(m_Window); //set context to window
			glfwSetWindowUserPointer(m_Window, this);
			glfwSetKeyCallback(m_Window, key_callback);
			glfwSetWindowSizeCallback(m_Window, window_resize);
			glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
			glfwSetCursorPosCallback(m_Window, cursor_position_callback);
			glfwSwapInterval(0.0);
			return true;
		}

		void window_resize(GLFWwindow *window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}

		void Window::isFullscreen()
		{

			const GLFWvidmode* mode = glfwGetVideoMode(m_Monitor);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
			GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "My Title", m_Monitor, NULL);
		}

		void Window::isWindowedFullscreen()
		{

		}

		bool Window::isKeyPressed(unsigned int keycode) const
		{
			//TODO: Log This
			if (keycode >= MAX_KEYS)
			{
				return false;
			}

			return m_Keys[keycode];
		}

		bool Window::isMouseButtonPressed(unsigned int button) const
		{
			//TODO: Log This
			if (button >= MAX_BUTTONS)
			{
				return false;
			}

			return m_MouseButtons[button];
		}

		void Window::getMousePosition(maths::vec2& xy) const
		{
			xy.x = mx;
			xy.y = my;
		}
		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);

			win->m_Keys[key] = action != GLFW_RELEASE;
		}

		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->m_MouseButtons[button] = action != GLFW_RELEASE;
		}

		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->mx = xpos;
			win->my = ypos;
		}
	}
}