#pragma once

#include <header.hpp>

class VeWindow{
	private:
		int			_width;
		int			_height;
		bool		_framebufferResized = false;
		GLFWwindow	*_window;

		static void	framebufferResizedCallback(GLFWwindow *window, int width, int height);
		void		initWindow(void);
	public:
		VeWindow(int width, int height);
		~VeWindow();

		// Getter
		VkExtent2D	getExtent(void);
		bool		shouldClose(void);
		bool		wasWindowResized(void);
		GLFWwindow	*getGLFWwindow(void) const;

		void		resetWindowResizedFlag(void);
		void		createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
};
