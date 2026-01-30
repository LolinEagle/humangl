#include <Controller.hpp>

using namespace vem;

double	gScroll = 0;

void	scrollCallback(GLFWwindow *win, double xoffset, double yoffset){
	gScroll = yoffset;
}

void	Controller::swingModel(const int &bodyPart, const float &swingAngle){
	auto	&torsoZ = _model[TORSO]->_transform.offset.z;

	// Upper
	auto	&part = _model[bodyPart]->_transform;
	float	sign = part.translation.x < 0.f ? -1.f : 1.f;

	part.offset.y = (swingAngle < 0.f ? swingAngle : -swingAngle) * .5f;
	part.offset.z = swingAngle * sign * 1.5f + torsoZ;
	part.rotation.x = swingAngle * sign;

	// Lower
	auto	&lowerPart = _model[bodyPart + 1]->_transform;

	lowerPart.offset.y = lowerPart.scale.y * 2.f * cos(swingAngle * sign) - (
		lowerPart.translation.y - part.translation.y - part.offset.y);
	lowerPart.offset.z = lowerPart.scale.y * 2.f * sin(swingAngle * sign) - (
		lowerPart.translation.z - part.translation.z - part.offset.z);
	lowerPart.rotation.x = swingAngle * sign;
}

void	Controller::resetModel(const int &bodyPart){
	auto	&part = _model[bodyPart]->_transform;

	part.rotation.x = 0.f;
	part.offset.z = 0.f;
	part.offset.y = 0.f;
}

Controller::Controller(GLFWwindow *win, VeGameObject::MapPtr &model)
: _isJumping(false), _isFalling(false), _model(model){
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

	double	newXpos = 0., newYpos = 0.;	// New cursor position
	vec3	rotate{};					// Rotate camera

	// Mouse input
	glfwGetKey(win, _keys.mouse) ? _mouseEnable = false : _mouseEnable = true;
	if (!_mouseEnable){
		glfwGetCursorPos(win, &newXpos, &newYpos);
		if (newXpos < 0. || newYpos < 0. || newXpos > WIDTH || newYpos > HEIGHT)
			return ;
		if (HEIGHT_HALF > newYpos + _lookOffset) rotate.x += 1.f;	// Up
		if (HEIGHT_HALF < newYpos - _lookOffset) rotate.x -= 1.f;	// Down
		if (WIDTH_HALF < newXpos + _lookOffset) rotate.y += 1.f;	// Right
		if (WIDTH_HALF > newXpos - _lookOffset) rotate.y -= 1.f;	// Left
		glfwSetCursorPos(win, WIDTH_HALF, HEIGHT_HALF);			// Lock cursor
	} else {
		if (glfwGetKey(win, _keys.camUp)) rotate.x += 1.f;		// Look up
		if (glfwGetKey(win, _keys.camDown)) rotate.x -= 1.f;	// Look down
		if (glfwGetKey(win, _keys.camRight)) rotate.y += 1.f;	// Look right
		if (glfwGetKey(win, _keys.camLeft)) rotate.y -= 1.f;	// Look left
	}

	// Rotation
	if (dot(rotate, rotate) > numeric_limits<float>::epsilon())
		go._transform.rotation += vec3(_lookSpeed * dt) * normalize(rotate);
	go._transform.rotation.x = clamp(go._transform.rotation.x, -1.5f, 1.5f);
	go._transform.rotation.y = mod(go._transform.rotation.y, TWO_PI);

	float		yaw = go._transform.rotation.y;
	const vec3	forwardDir{sin(yaw), 0.f, cos(yaw)};		// Forward
	const vec3	rightDir{forwardDir.z, 0.f, -forwardDir.x};	// Right
	const vec3	upDir{0.f, -1.f, 0.f};						// Up
	vec3		moveDir{};									// Move

	// Keyboard input
	if (glfwGetKey(win, _keys.moveForward)) moveDir += forwardDir;
	if (glfwGetKey(win, _keys.moveLeft)) moveDir -= rightDir;
	if (glfwGetKey(win, _keys.moveBackward)) moveDir -= forwardDir;
	if (glfwGetKey(win, _keys.moveRight)) moveDir += rightDir;
	if (glfwGetKey(win, _keys.moveUp)) moveDir += upDir;
	if (glfwGetKey(win, _keys.moveDown)) moveDir -= upDir;

	// Mouse scroll
	if (gScroll > 0.){
		moveDir += upDir;// Move up
		gScroll -= .1;
	}
	if (gScroll < 0.){
		moveDir -= upDir;// Move down
		gScroll += .1;
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

	// Jump
	if (_model.size() == 0)
		return ;
	const float	&_currentY =
		_model.begin()->second->_transform.jumpOffset;
	if (glfwGetKey(win, _keys.space) && _isFalling == false) _isJumping = true;
	if (_isJumping){
		for (auto &obj: _model)
			obj.second->_transform.jumpOffset -= _jumpSpeed * dt;
		if (_currentY <= -_jumpHeight){
			_isJumping = false;
			_isFalling = true;
		}
	} else if (_isFalling){
		for (auto &obj: _model)
			obj.second->_transform.jumpOffset += _jumpSpeed * dt;
		if (_currentY >= 0.f)
			_isFalling = false;
	}

	// Walk
	for (int i = 0; i < BODY_LENGTH; i++){
		if (_model.find(i) == _model.end())
			return ;
	}
	if (glfwGetKey(win, _keys.walk)){
		// Move torso, head and hat
		auto	&torsoZ = _model[TORSO]->_transform.offset.z;

		torsoZ += _walkSpeed * dt;
		for (int i = HEAD; i <= HAT; i++)
			_model[i]->_transform.offset.z = torsoZ;

		// Move arms and legs
		float	time = static_cast<float>(glfwGetTime());
		float	swingAngle = radians(35.f * sin(3.f * time));

		for (int i = LEFT_UPPER_ARM; i <= RIGHT_UPPER_LEG; i += 2)
			swingModel(i, swingAngle);
	} else {
		for (int i = 0; i < BODY_LENGTH; i++)
			resetModel(i);
	}
}
