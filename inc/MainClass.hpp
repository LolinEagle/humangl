#pragma once

#include <Controller.hpp>
#include <SimpleRender.hpp>
#include <PointLight.hpp>
#include <VeBuffer.hpp>
#include <VeCamera.hpp>
#include <VeDescriptors.hpp>
#include <VeDevice.hpp>
#include <VeGameObject.hpp>
#include <VeRenderer.hpp>
#include <VeWindow.hpp>

using namespace std;

class MainClass{
private:
	VeWindow		_veWindow{WIDTH, HEIGHT};
	VeDevice		_veDevice{_veWindow};
	VeRenderer		_veRenderer{_veWindow, _veDevice};
	int				_color;
	int				_texture;

	VeGameObject	*_skybox = nullptr;

	unique_ptr<VeDescriptorPool>	_globalPool{};
	VeGameObject::Map				_gameObjects;
	VeGameObject::MapPtr			_model;
	const vem::vec3					_cameraTranslation = {0.f, -8.f, 24.f};
	const vem::vec3					_cameraRotation = {0.f, M_PI, 0.f};
	const float						_near = .1f;
	const float						_far = 128.f;

	const uint	loadGameObjects(
		const string &filepath,
		vem::vec3 translation,
		vem::vec3 scale,
		int bodyPart = -1
	);
	void		loadHumanGL(void);
	void		loadScop(void);

	const uint loadCubeObject(
		vem::vec3 translation,
		vem::vec3 scale,
		TexIdRaw texId,
		int bodyPart = -1
	);

	// this is a function that setups the texture ids for the ubo
	//      -> this could allow us to change textures at runtime!
	static void setTextureUvs(TexIdData &ubo);
public:
	using cV3 = const vem::vec3;

	MainClass(int scene, int color, int texture);
	~MainClass();

	void	run(void);
};
