#include <MainClass.hpp>

using namespace std;
using seconds_float = chrono::duration<float, chrono::seconds::period>;

void	MainClass::loadGameObjects(
	const string &filepath,
	vem::vec3 translation,
	vem::vec3 scale,
	int bodyPart = -1
){
	shared_ptr<VeModel>	veModel = VeModel::createModelFromFile(
		_veDevice, "model/" + filepath + ".obj", _color, _texture
	);
	auto	gameObject = VeGameObject::createGameObject();

	gameObject._model = veModel;
	gameObject._transform.translation = translation;
	gameObject._transform.scale = scale;
	_gameObjects.emplace(gameObject.getId(), move(gameObject));
	if (bodyPart != -1)
		_model.emplace(bodyPart, &_gameObjects[gameObject.getId()]);
}

vem::vec3	makeT(
	const vem::vec3 &base,
	const float &baseScale,
	const float &newScale,
	float offsetX = 0.f
){
	return (vem::vec3(
		base.x - offsetX, base.y - baseScale - newScale , base.z
	));
}

void	MainClass::loadHumanGL(void){
	// Scale
	cV3	torsoS{2.f, 3.f, 1.f};
	cV3	headS{2.f, 2.f, 2.f};
	cV3	hatBaseS{2.5f, .25f, 2.5f};
	cV3	hatS{2.f, .5f, 2.f};
	cV3	limbsS{1.f, 1.5f, 1.f};
	cV3	groundS{32.f, .1f, 32.f};

	// Translation
	cV3	torso{0.f, -7.5f, 0.f};
	cV3	head = makeT(torso, torsoS.y, headS.y);
	cV3	hatBase = makeT(head, headS.y, hatBaseS.y);
	cV3	hat = makeT(hatBase, hatBaseS.y, hatS.y);
	cV3	leftUpperArm = makeT(head, -headS.y, -limbsS.y, torsoS.x + limbsS.x);
	cV3	leftLowerArm = makeT(leftUpperArm, -limbsS.y, -limbsS.y);
	cV3	rightUpperArm = makeT(head, -headS.y, -limbsS.y, -torsoS.x - limbsS.x);
	cV3	rightLowerArm = makeT(rightUpperArm, -limbsS.y, -limbsS.y);
	cV3	leftUpperLeg = makeT(torso, -torsoS.y, -limbsS.y, limbsS.x);
	cV3	leftLowerLeg = makeT(leftUpperLeg, -limbsS.y, -limbsS.y);
	cV3	rightUpperLeg = makeT(torso, -torsoS.y, -limbsS.y, -limbsS.x);
	cV3	rightLowerLeg = makeT(rightUpperLeg, -limbsS.y, -limbsS.y);
	cV3	ground = makeT(torso, -torsoS.y - groundS.y, -limbsS.y * 4.f);

	// Body part
	loadGameObjects("cube", torso, torsoS, TORSO);
	loadGameObjects("cube", head, headS, HEAD);
	loadGameObjects("cube", hatBase, hatBaseS, HAT_BASE);
	loadGameObjects("cube", hat, hatS, HAT);
	loadGameObjects("cube", leftUpperArm, limbsS, LEFT_UPPER_ARM);
	loadGameObjects("cube", leftLowerArm, limbsS, LEFT_LOWER_ARM);
	loadGameObjects("cube", rightUpperArm, limbsS, RIGHT_UPPER_ARM);
	loadGameObjects("cube", rightLowerArm, limbsS, RIGHT_LOWER_ARM);
	loadGameObjects("cube", leftUpperLeg, limbsS, LEFT_UPPER_LEG);
	loadGameObjects("cube", leftLowerLeg, limbsS, LEFT_LOWER_LEG);
	loadGameObjects("cube", rightUpperLeg, limbsS, RIGHT_UPPER_LEG);
	loadGameObjects("cube", rightLowerLeg, limbsS, RIGHT_LOWER_LEG);

	// Ground
	loadGameObjects("cube", ground, groundS);
}

void	MainClass::loadScop(void){
	loadGameObjects("42", {-2.f, -.2f,  1.f}, {1.f, -1.f, 1.f});
	loadGameObjects("cube", {-2.f,  0.f, -2.f}, {1.f,  1.f, 1.f});
	loadGameObjects("teapot", {2.f,  1.1f,  2.f}, {1.f, -1.f, 1.f});
	loadGameObjects("teapot2", {2.f, -.3f, -2.f}, {1.f, -1.f, 1.f});
	loadGameObjects("cube", {0.f,  1.2f,  0.f}, {8.f,  .1f, 8.f});
}

MainClass::MainClass(int scene, int color, int texture)
: _color(color), _texture(texture){
	_globalPool = VeDescriptorPool::Builder(_veDevice)
		.setMaxSets(MAX_FRAMES * 2)
		.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES)
		.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES)
		.build();
	if (scene == HUMANGL) loadHumanGL();
	else if (scene == SCOP) loadScop();
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
		.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			VK_SHADER_STAGE_ALL_GRAPHICS)
		.addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 
			VK_SHADER_STAGE_FRAGMENT_BIT)
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
	Controller	controller{_veWindow.getGLFWwindow(), _model};

	// Time
	auto		currentTime = chrono::high_resolution_clock::now();
	auto		newTime = currentTime;
	float		frameTime;
	int			frameIndex;

	// Default value for camera
	viewerObject._transform.translation = _cameraTranslation;
	viewerObject._transform.rotation = _cameraRotation;
	while (!_veWindow.shouldClose()){
		glfwPollEvents();

		// Time
		newTime = chrono::high_resolution_clock::now();
		frameTime = seconds_float(newTime - currentTime).count();
		currentTime = newTime;

		controller.moveInPlaneXZ(
			_veWindow.getGLFWwindow(), frameTime, viewerObject
		);
		camera.setViewYXZ(
			viewerObject._transform.translation,
			viewerObject._transform.rotation
		);
		aspect = _veRenderer.getAspectRatio();
		camera.setPerspectiveProjection(vem::radians(50.), aspect, _near, _far);
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
			ubo.textureOn = controller.getTextureOn();
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
