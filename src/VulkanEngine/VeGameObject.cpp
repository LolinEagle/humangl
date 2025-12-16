#include <VeGameObject.hpp>

vem::mat4	TransformComponent::mat4(void){
	// Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
	// Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
	const float	c3 = cos(rotation.z);
	const float	s3 = sin(rotation.z);
	const float	c2 = cos(rotation.x);
	const float	s2 = sin(rotation.x);
	const float	c1 = cos(rotation.y);
	const float	s1 = sin(rotation.y);

	return (vem::mat4{
		{
			scale.x * (c1 * c3 + s1 * s2 * s3),
			scale.x * (c2 * s3),
			scale.x * (c1 * s2 * s3 - c3 * s1),
			0.f,
		},{
			scale.y * (c3 * s1 * s2 - c1 * s3),
			scale.y * (c2 * c3),
			scale.y * (c1 * c3 * s2 + s1 * s3),
			0.f,
		},{
			scale.z * (c2 * s1),
			scale.z * (-s2),
			scale.z * (c1 * c2),
			0.f,
		},{
			translation.x, translation.y, translation.z, 1.0f
		}
	});
}

vem::mat3	TransformComponent::normalMatrix(void){
	const float	c3 = cos(rotation.z);
	const float	s3 = sin(rotation.z);
	const float	c2 = cos(rotation.x);
	const float	s2 = sin(rotation.x);
	const float	c1 = cos(rotation.y);
	const float	s1 = sin(rotation.y);
	const vem::vec3	invScale = vem::vec3(1.f) / scale;

	return (vem::mat3{
		{
			invScale.x * (c1 * c3 + s1 * s2 * s3),
			invScale.x * (c2 * s3),
			invScale.x * (c1 * s2 * s3 - c3 * s1),
		},{
			invScale.y * (c3 * s1 * s2 - c1 * s3),
			invScale.y * (c2 * c3),
			invScale.y * (c1 * c3 * s2 + s1 * s3),
		},{
			invScale.z * (c2 * s1),
			invScale.z * (-s2),
			invScale.z * (c1 * c2),
		}
	});
}

VeGameObject::VeGameObject(uint32_t id) : _id(id){
}

VeGameObject::VeGameObject(void){
	*this = createGameObject();
}

VeGameObject::~VeGameObject(){
}

VeGameObject	VeGameObject::createGameObject(void){
	static uint32_t	currentId = 0;

	return (VeGameObject(currentId++));
}

VeGameObject	VeGameObject::makePointLight(float intensity, float radius, vem::vec3 color){
	VeGameObject	obj = VeGameObject::createGameObject();
	obj._color = color;
	obj._transform.scale.x = radius;
	obj._pointLight = make_unique<PointLightComponent>();
	obj._pointLight->lightIntensity = intensity;

	return (obj);
}

const uint32_t	VeGameObject::getId(void){
	return (_id);
}
