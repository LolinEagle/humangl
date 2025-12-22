#include <MainClass.hpp>

using namespace std;
using seconds_float = chrono::duration<float, chrono::seconds::period>;

void	MainClass::loadGameObjects(
	const string &filepath, vem::vec3 translation, vem::vec3 scale,
	const int &color, const int &texture
){
	shared_ptr<VeModel>	veModel = VeModel::createModelFromFile(
		_veDevice, "model/" + filepath + ".obj", color, texture
	);
	auto	gameObject = VeGameObject::createGameObject();

	gameObject._model = veModel;
	gameObject._transform.translation = translation;
	gameObject._transform.scale = scale;
	_gameObjects.emplace(gameObject.getId(), move(gameObject));
}

MainClass::MainClass(const int &color, const int &texture){
	_globalPool = VeDescriptorPool::Builder(_veDevice)
		.setMaxSets(MAX_FRAMES * 2)
		.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES)
		.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES)
		.build();

	const vem::vec3	scale{2.f, 3.f, 2.f};

	// Head & Torso
	loadGameObjects("cube", {0.f, -25.f, 0.f}, {4.f, 4.f, 4.f}, color, texture);
	loadGameObjects("cube", {0.f, -15.f, 0.f}, {4.f, 6.f, 2.f}, color, texture);

	// Arms
	loadGameObjects("cube", {6.f, -18.f, 0.f}, scale, color, texture);
	loadGameObjects("cube", {-6.f, -18.f, 0.f}, scale, color, texture);
	loadGameObjects("cube", {6.f, -12.f, 0.f}, scale, color, texture);
	loadGameObjects("cube", {-6.f, -12.f, 0.f}, scale, color, texture);

	// Legs
	loadGameObjects("cube", {2.f, -6.f, 0.f}, scale, color, texture);
	loadGameObjects("cube", {-2.f, -6.f, 0.f}, scale, color, texture);
	loadGameObjects("cube", {2.f, 0.f, 0.f}, scale, color, texture);
	loadGameObjects("cube", {-2.f, 0.f, 0.f}, scale, color, texture);

	// Ground
	loadGameObjects("cube", {0.f, 4.f, 0.f}, {16.f, 1.f, 16.f}, color, texture);
}

MainClass::~MainClass(){
}

void	MainClass::run(void){
	vector<unique_ptr<VeBuffer>>	uboBuffers(MAX_FRAMES);
	for (int i = 0; i < uboBuffers.size(); i++){
		uboBuffers[i] = make_unique<VeBuffer>(
			_veDevice,
			sizeof(GlobalUbo),
			1,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			_veDevice._properties.limits.minUniformBufferOffsetAlignment
		);
		uboBuffers[i]->map();
	}

	auto	globalSetLayout = VeDescriptorSetLayout::Builder(_veDevice)
		.addBinding(0, (VkDescriptorType)6, VK_SHADER_STAGE_ALL_GRAPHICS)
		.addBinding(1, (VkDescriptorType)1, VK_SHADER_STAGE_FRAGMENT_BIT)
		.build();
	vector<VkDescriptorSet>	globalDescriptorSets(MAX_FRAMES);
	for (int i = 0; i < globalDescriptorSets.size(); i++){
		VkDescriptorBufferInfo	buffer = uboBuffers[i]->descriptorBufferInfo();
		VkDescriptorImageInfo	image;
		for (const auto &[id, gameObject] : _gameObjects){
			if (gameObject._model){
				image = gameObject._model->descriptorImageInfo();
				break ;
			}
		}
		VeDescriptorWriter(*globalSetLayout, *_globalPool)
			.writeBuffer(0, &buffer)
			.writeImage(1, &image)
			.build(globalDescriptorSets[i]);
	}

	SimpleRender	simpleRenderSystem{
		_veDevice,
		_veRenderer.getSwapchainRenderPass(),
		globalSetLayout->getDescriptorSetLayout()
	};
	PointLight		pointLightSystem{
		_veDevice,
		_veRenderer.getSwapchainRenderPass(),
		globalSetLayout->getDescriptorSetLayout()
	};

	VeCamera	camera{};
	float		aspect;
	auto		viewerObject = VeGameObject::createGameObject();
	Controller	cameraController{_veWindow.getGLFWwindow()};

	// Time
	auto		currentTime = chrono::high_resolution_clock::now();
	auto		newTime = currentTime;
	float		frameTime;
	int			frameIndex;

	// Default value for camera
	viewerObject._transform.translation = {-8.f, -2.0f, .0f};
	viewerObject._transform.rotation = {.0f, 1.5f, .0f};
	while (!_veWindow.shouldClose()){
		glfwPollEvents();

		// Time
		newTime = chrono::high_resolution_clock::now();
		frameTime = seconds_float(newTime - currentTime).count();
		currentTime = newTime;

		cameraController.moveInPlaneXZ(
			_veWindow.getGLFWwindow(), frameTime, viewerObject
		);
		camera.setViewYXZ(
			viewerObject._transform.translation,
			viewerObject._transform.rotation
		);
		aspect = _veRenderer.getAspectRatio();
		camera.setPerspectiveProjection(vem::radians(50.), aspect, .1f, 64.f);
		if (auto commandBuffer = _veRenderer.beginFrame()){
			frameIndex = _veRenderer.getCurrentFrameIndex();
			FrameInfo	frameInfo{
				frameIndex,
				frameTime,
				commandBuffer,
				camera,
				globalDescriptorSets[frameIndex],
				_gameObjects
			};

			// Update
			GlobalUbo	ubo{};
			ubo.projection = camera.getProjection();
			ubo.view = camera.getView();
			ubo.inverseView = camera.getInverseView();
			ubo.textureOn = cameraController.getTextureOn();
			pointLightSystem.update(frameInfo, ubo);
			uboBuffers[frameIndex]->writeToBuffer(&ubo);
			uboBuffers[frameIndex]->flush();

			// Render
			_veRenderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderObjects(frameInfo);
			pointLightSystem.render(frameInfo);
			_veRenderer.endSwapChainRenderPass(commandBuffer);
			_veRenderer.endFrame();
		}
	}
	vkDeviceWaitIdle(_veDevice.device());
}
