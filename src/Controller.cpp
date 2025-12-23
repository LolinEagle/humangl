#include <Controller.hpp>

using namespace vem;

double	gScroll = 0;

void	scrollCallback(GLFWwindow *win, double xoffset, double yoffset){
	gScroll = yoffset;
}

Controller::Controller(GLFWwindow *win){
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetScrollCallback(win, scrollCallback);
}

Controller::~Controller(){
}

bool	Controller::getTextureOn(void) const {
	return (_textureOn);
}

void	Controller::moveInPlaneXZ(GLFWwindow *win, float dt, VeGameObject &go){
	// Escape
	if (glfwGetKey(win, _keys.escape)) glfwSetWindowShouldClose(win, GLFW_TRUE);

	double	newXpos, newYpos;	// New cursor position
	vec3	rotate{0};			// Rotate camera

	// Mouse input
	glfwGetCursorPos(win, &newXpos, &newYpos);
	if (newXpos < 0 || newYpos < 0 || newXpos > WIDTH || newYpos > HEIGHT)
		return ;
	if (HEIGHT_HALF > newYpos + _lookOffset) rotate.x += 1.f;	// Look up
	if (HEIGHT_HALF < newYpos - _lookOffset) rotate.x -= 1.f;	// Look down
	if (WIDTH_HALF < newXpos + _lookOffset) rotate.y += 1.f;	// Look right
	if (WIDTH_HALF > newXpos - _lookOffset) rotate.y -= 1.f;	// Look left
	glfwSetCursorPos(win, WIDTH_HALF, HEIGHT_HALF);				// Lock cursor

	// Rotation
	if (dot(rotate, rotate) > numeric_limits<float>::epsilon())
		go._transform.rotation += vec3(_lookSpeed * dt) * normalize(rotate);
	go._transform.rotation.x = clamp(go._transform.rotation.x, -1.5f, 1.5f);
	go._transform.rotation.y = mod(go._transform.rotation.y, TWO_PI);

	float		yaw = go._transform.rotation.y;
	const vec3	forwardDir{sin(yaw), 0.f, cos(yaw)};		// Forward
	const vec3	rightDir{forwardDir.z, 0.f, -forwardDir.x};	// Right
	const vec3	upDir{0.f, -1.f, 0.f};						// Up
	vec3		moveDir{0};									// Move

	// Keyboard input
	if (glfwGetKey(win, _keys.moveForward)) moveDir += forwardDir;
	if (glfwGetKey(win, _keys.moveLeft)) moveDir -= rightDir;
	if (glfwGetKey(win, _keys.moveBackward)) moveDir -= forwardDir;
	if (glfwGetKey(win, _keys.moveRight)) moveDir += rightDir;

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
	if (dot(moveDir, moveDir) > numeric_limits<float>::epsilon())
		go._transform.translation += vec3(_moveSpeed * dt) * normalize(moveDir);

	// Texture on
	if (glfwGetKey(win, _keys.texture) && _textureOnPress == false){
		_textureOn = !_textureOn;
		_textureOnPress = true;
	}
	if (glfwGetKey(win, _keys.texture) == GLFW_RELEASE)
		_textureOnPress = false;
}
