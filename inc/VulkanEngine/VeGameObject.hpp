#pragma once

#include <VeModel.hpp>

using namespace std;

struct TransformComponent{
	vem::vec3	translation{};
	vem::vec3	scale{1.f, 1.f, 1.f};
	vem::vec3	rotation{};

	vem::mat4	mat4(void);
	vem::mat3	normalMatrix(void);	
};

struct PointLightComponent{
	float	lightIntensity = 1.f;
};

class VeGameObject{
	private:
		uint32_t	_id;

		VeGameObject(uint32_t id);
	public:
		using Map = unordered_map<uint32_t, VeGameObject>;

		vem::vec3						_color{};
		TransformComponent				_transform{};
		shared_ptr<VeModel>				_model{};
		unique_ptr<PointLightComponent>	_pointLight = nullptr;

		VeGameObject(void);
		~VeGameObject();

		VeGameObject(const VeGameObject &) = delete;
		VeGameObject &operator=(const VeGameObject &) = delete;
		VeGameObject(VeGameObject &&) = default;
		VeGameObject &operator=(VeGameObject &&) = default;

		static VeGameObject	createGameObject(void);
		static VeGameObject	makePointLight(float intensity, float radius, vem::vec3 color);
		const uint32_t		getId(void);
};
