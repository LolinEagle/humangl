#pragma once

#include <VeGameObject.hpp>

class Controller{
private:
	struct KeyMappings{
		const int	moveForward = GLFW_KEY_W;
		const int	moveLeft = GLFW_KEY_A;
		const int	moveBackward = GLFW_KEY_S;
		const int	moveRight = GLFW_KEY_D;
		const int	texture = GLFW_KEY_F;
		const int	escape = GLFW_KEY_ESCAPE;
		const int	space = GLFW_KEY_SPACE;
	};

	const KeyMappings	_keys{};
	const float			_moveSpeed = 4.f;
	const float			_lookSpeed = 4.f;
	const double		_lookOffset = 2.;
	bool				_textureOn = false;
	bool				_textureOnPress;

	// Jump
	const float			_jumpSpeed = 16.f;
	const float			_jumpHeight = 3.f;
	bool				_isJumping;
	bool				_isFalling;
	VeGameObject::Map	&_gameObjects;
public:
	Controller(GLFWwindow *win, VeGameObject::Map &gameObjects);
	~Controller();

	bool	getTextureOn(void) const;
	void	moveInPlaneXZ(GLFWwindow *win, float dt, VeGameObject &go);
};
