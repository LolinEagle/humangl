#pragma once

#include <VeGameObject.hpp>

class Controller{
	private:
		struct KeyMappings{
			int	moveForward = GLFW_KEY_W;
			int	moveLeft = GLFW_KEY_A;
			int	moveBackward = GLFW_KEY_S;
			int	moveRight = GLFW_KEY_D;
			int texture = GLFW_KEY_F;
			int	escape = GLFW_KEY_ESCAPE;
		};

		const KeyMappings	_keys{};
		const float			_moveSpeed = 4.f;
		const float			_lookSpeed = 16.f;
		const double		_lookOffset = 2.;
		bool				_textureOn = false;
		bool				_textureOnPress;
	public:
		Controller(GLFWwindow *window);
		~Controller();

		bool	getTextureOn(void) const;
		void	moveInPlaneXZ(GLFWwindow *window, float dt, VeGameObject &gameObject);
};
