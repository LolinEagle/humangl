#pragma once

#include <header.hpp>

class VeWindow{
private:
	int			_width;
	int			_height;
	bool		_framebufferResized = false;
	GLFWwindow	*_window;

	static void	framebufferResizedCallback(GLFWwindow *window, int w, int h);
	void		initWindow(void);
public:
	VeWindow(int w, int h);
	~VeWindow();

	// Getter
	VkExtent2D	getExtent(void);
	bool		shouldClose(void);
	bool		wasWindowResized(void);
	GLFWwindow	*getGLFWwindow(void) const;

	void		resetWindowResizedFlag(void);
	void		createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
};
