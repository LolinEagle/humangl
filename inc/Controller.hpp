#pragma once

#include <VeGameObject.hpp>

class Controller{
private:
	struct KeyMappings{
		const int	moveForward = GLFW_KEY_W;
		const int	moveLeft = GLFW_KEY_A;
		const int	moveBackward = GLFW_KEY_S;
		const int	moveRight = GLFW_KEY_D;
		const int	walk = GLFW_KEY_E;
		const int	texture = GLFW_KEY_F;
		const int	escape = GLFW_KEY_ESCAPE;
		const int	space = GLFW_KEY_SPACE;
	};

	const KeyMappings		_keys{};
	VeGameObject::MapPtr	&_model;

	// Main controle
	const float	_moveSpeed = 4.f;
	const float	_lookSpeed = 4.f;
	const float	_lookOffset = 2.f;
	bool		_textureOn = false;
	bool		_textureOnPress;

	// Jump
	const float	_jumpSpeed = 16.f;
	const float	_jumpHeight = 3.f;
	bool		_isJumping;
	bool		_isFalling;
public:
	Controller(GLFWwindow *win, VeGameObject::MapPtr &model);
	~Controller();

	bool	getTextureOn(void) const;
	void	moveInPlaneXZ(GLFWwindow *win, float dt, VeGameObject &go);
};
