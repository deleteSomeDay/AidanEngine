#pragma once
#include "../../AidanEngine.h"
#include "../maths/maths.h"
namespace engine {

	namespace graphics {

#define MAX_KEYS 1024
#define MAX_BUTTONS 32
		class Window
		{
		public:

		private:

			const char *m_Title;
			int m_Width, m_Height;
			GLFWwindow *m_Window;
			GLFWmonitor *m_Monitor;
			bool m_Closed;

			bool m_Keys[MAX_KEYS];
			bool m_MouseButtons[MAX_BUTTONS];
			double mx, my;
		public:
			Window(const char *name, int width, int height);
			~Window();
			bool closed() const;
			void update();
			void Clear() const;

			inline int getWidth() const { return m_Width; }
			inline int getHeight() const { return m_Height; }

			void isWindowedFullscreen();
			void isFullscreen();

			bool isKeyPressed(unsigned int keycode) const;
			bool isMouseButtonPressed(unsigned int button) const;
			void getMousePosition(maths::vec2& xy) const;

		private:
			bool init();
			friend static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			friend static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
			friend static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		};
	}
}
