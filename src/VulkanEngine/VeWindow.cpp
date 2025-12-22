#include <VeWindow.hpp>

using namespace std;

void		VeWindow::framebufferResizedCallback(GLFWwindow *window, int w, int h){
	auto	veWindow = reinterpret_cast<VeWindow*>(glfwGetWindowUserPointer(window));

	veWindow->_framebufferResized = true;
	veWindow->_width = w;
	veWindow->_height = h;
}

void		VeWindow::initWindow(void){
	if (!glfwInit())
		throw (runtime_error("failed to initialize GLFW"));
	if (!glfwVulkanSupported())
		throw (runtime_error("vulkan not supported"));
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	_window = glfwCreateWindow(_width, _height, TITLE, nullptr, nullptr);
	if (!_window)
		throw (runtime_error("failed to create GLFW window"));
	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, framebufferResizedCallback);
}

VeWindow::VeWindow(int w, int h) : _width(w), _height(h){
	initWindow();
}

VeWindow::~VeWindow(){
	glfwDestroyWindow(_window);
	glfwTerminate();
}

VkExtent2D	VeWindow::getExtent(void){
	return {static_cast<uint32_t>(_width), static_cast<uint32_t>(_height)};
}

bool		VeWindow::shouldClose(void){
	return (glfwWindowShouldClose(_window));
}

bool		VeWindow::wasWindowResized(void){
	return (_framebufferResized);
}

GLFWwindow	*VeWindow::getGLFWwindow(void) const {
	return (_window);
}

void		VeWindow::resetWindowResizedFlag(void){
	_framebufferResized = false;
}

void		VeWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
	if (glfwCreateWindowSurface(instance, _window, nullptr, surface) != 0)
		throw (runtime_error("failed to create window surface"));
}
