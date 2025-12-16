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
		VeWindow	_veWindow{WIDTH, HEIGHT};
		VeDevice	_veDevice{_veWindow};
		VeRenderer	_veRenderer{_veWindow, _veDevice};

		unique_ptr<VeDescriptorPool>	_globalPool{};
		VeGameObject::Map				_gameObjects;

		void	loadGameObjects(
			const string &filepath, vem::vec3 translation, vem::vec3 scale,
			const int &color, const int &texture
		);
	public:
		MainClass(const int &scene, const int &color, const int &texture);
		~MainClass();

		void	run(void);
};
