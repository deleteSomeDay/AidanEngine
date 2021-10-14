#pragma once
#include "../../AidanEngine.h"

namespace engine
{

	struct WindowProperties
	{
		uint width, height;
		bool fullscreen;
		bool vsync;
	};

	class Window
	{
	private:
		static std::map<void *, Window*> s_Handles;

		std::string m_Title;
		WindowProperties m_Properties;
		bool m_Closed;
		void* m_Handle;

		bool m_Vsync;
		WindowEventCallback m_EventCallback;
		InputManager* m_InputManager;

	public:

		Window(const std::string& name, const WindowProperties& properties);
		~Window();

		bool closed() const;
		void update();
		void clear() const;

		void setTitle(const std::string& title);

		inline uint getWidth() const { return m_Properties.width; }
		inline uint getHeight() const { return m_Properties.height; }

		void setVsync(bool enabled);

		inline bool isVsync() const { return m_Vsync; }

		void setEventCallback(const WindowEventCallback& callback);

		static void RegisterWindowClass(void* handle, Window* window);
		static Window* GetWindowClass(void* handle);
	private:

		bool init();

		bool platformInit();
		void platformUpdate();

		friend void resizeCallback(Window* window, int32 width, int32 height);
		friend void focusCallback(Window* window, bool focused);
	};
}