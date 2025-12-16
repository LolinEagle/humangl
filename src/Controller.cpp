#include <Controller.hpp>

double	gScroll = 0;

void	scrollCallback(GLFWwindow *window, double xoffset, double yoffset){
	gScroll = yoffset;
}

Controller::Controller(GLFWwindow *window){
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetScrollCallback(window, scrollCallback);
}

Controller::~Controller(){
}

bool	Controller::getTextureOn(void) const {
	return (_textureOn);
}

void	Controller::moveInPlaneXZ(GLFWwindow *window, float dt, VeGameObject &gameObject){
	// Escape
	if (glfwGetKey(window, _keys.escape) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);

	double		newXpos, newYpos;	// New cursor position
	vem::vec3	rotate{0};			// Rotate camera

	// Mouse input
	glfwGetCursorPos(window, &newXpos, &newYpos);
	if (newXpos < 0 || newYpos < 0 || newXpos > WIDTH || newYpos > HEIGHT)// If cursor not in window
		return ;
	if (HEIGHT_HALF > newYpos + _lookOffset) rotate.x += 1.f;	// Look up
	if (HEIGHT_HALF < newYpos - _lookOffset) rotate.x -= 1.f;	// Look down
	if (WIDTH_HALF < newXpos + _lookOffset) rotate.y += 1.f;	// Look right
	if (WIDTH_HALF > newXpos - _lookOffset) rotate.y -= 1.f;	// Look left
	glfwSetCursorPos(window, WIDTH_HALF, HEIGHT_HALF);			// Lock cursor

	// Rotation
	if (vem::dot(rotate, rotate) > numeric_limits<float>::epsilon())
		gameObject._transform.rotation += vem::vec3(_lookSpeed * dt) * vem::normalize(rotate);
	gameObject._transform.rotation.x = vem::clamp(gameObject._transform.rotation.x, -1.5f, 1.5f);
	gameObject._transform.rotation.y = vem::mod(gameObject._transform.rotation.y, TWO_PI);

	float			yaw = gameObject._transform.rotation.y;
	const vem::vec3	forwardDir{sin(yaw), 0.f, cos(yaw)};		// Forward direction
	const vem::vec3	rightDir{forwardDir.z, 0.f, -forwardDir.x};	// Right direction
	const vem::vec3	upDir{0.f, -1.f, 0.f};						// Up direction
	vem::vec3		moveDir{0};									// Move direction

	// Keyboard input
	if (glfwGetKey(window, _keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;	// Move forward
	if (glfwGetKey(window, _keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;		// Move left
	if (glfwGetKey(window, _keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;// Move backward
	if (glfwGetKey(window, _keys.moveRight) == GLFW_PRESS) moveDir += rightDir;		// Move right

	// Mouse scroll
	if (gScroll > 0){
		moveDir += upDir;// Move up
		gScroll -= 0.1;
	}
	if (gScroll < 0){
		moveDir -= upDir;// Move down
		gScroll += 0.1;
	}

	// Translation
	if (vem::dot(moveDir, moveDir) > numeric_limits<float>::epsilon())
		gameObject._transform.translation += vem::vec3(_moveSpeed * dt) * vem::normalize(moveDir);

	// Texture on
	if (glfwGetKey(window, _keys.texture) == GLFW_PRESS && _textureOnPress == false){
		_textureOn = !_textureOn;
		_textureOnPress = true;
	}
	if (glfwGetKey(window, _keys.texture) == GLFW_RELEASE)
		_textureOnPress = false;
}
