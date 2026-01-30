#include "SkyboxRender.hpp"
#include "TexIdUv.hpp"
#include "header.hpp"
#include <MainClass.hpp>

using namespace std;
using seconds_float = chrono::duration<float, chrono::seconds::period>;

static const TexUvMapping idMap{};

// why << 3 ? look a tht comment on the textureIdRaw
//static_assert(MAX_TEX_ID >= (_TEX_LAST << 3), "The textureId buffer is too small");

const uint	MainClass::loadGameObjects(
	const string &filepath,
	vem::vec3 translation,
	vem::vec3 scale,
	int bodyPart
){
	shared_ptr<VeModel>	veModel = VeModel::createModelFromFile(
		_veDevice, "model/" + filepath + ".obj", _color, _texture
	);
	auto	gameObject = VeGameObject::createGameObject();

	gameObject._model = veModel;
	gameObject._transform.translation = translation;
	gameObject._transform.scale = scale;
	_gameObjects.emplace(gameObject.getId(), std::move(gameObject));
	if (bodyPart != -1)
		_model.emplace(bodyPart, &_gameObjects[gameObject.getId()]);
	return gameObject.getId();
}

const uint MainClass::loadCubeObject(
	vem::vec3 translation,
	vem::vec3 scale,
	TexIdRaw texId,
	int bodyPart
){
	shared_ptr<VeModel>	veModel = VeModel::createCubeModel(_veDevice, texId);
	auto	gameObject = VeGameObject::createGameObject();

	gameObject._model = veModel;
	gameObject._transform.translation = translation;
	gameObject._transform.scale = scale;
	_gameObjects.emplace(gameObject.getId(), std::move(gameObject));
	if (bodyPart != -1)
		_model.emplace(bodyPart, &_gameObjects[gameObject.getId()]);
	return gameObject.getId();

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
	loadCubeObject(torso,         torsoS,           TEX_TORSO,  TORSO         );
	loadCubeObject(head,          headS,             TEX_HEAD,  HEAD          );
	loadCubeObject(hatBase,       hatBaseS,      TEX_HAT_BASE,  HAT_BASE      );
	loadCubeObject(hat,           hatS,           TEX_HAT_RIM,  HAT           );
	loadCubeObject(leftUpperArm,  limbsS,  TEX_LEFT_UPPER_ARM,  LEFT_UPPER_ARM);
	loadCubeObject(leftLowerArm,  limbsS,  TEX_LEFT_LOWER_ARM,  LEFT_LOWER_ARM);
	loadCubeObject(rightUpperArm, limbsS, TEX_RIGHT_UPPER_ARM, RIGHT_UPPER_ARM);
	loadCubeObject(rightLowerArm, limbsS, TEX_RIGHT_LOWER_ARM, RIGHT_LOWER_ARM);
	loadCubeObject(leftUpperLeg,  limbsS,  TEX_LEFT_UPPER_LEG,  LEFT_UPPER_LEG);
	loadCubeObject(leftLowerLeg,  limbsS,  TEX_LEFT_LOWER_LEG,  LEFT_LOWER_LEG);
	loadCubeObject(rightUpperLeg, limbsS, TEX_RIGHT_UPPER_LEG, RIGHT_UPPER_LEG);
	loadCubeObject(rightLowerLeg, limbsS, TEX_RIGHT_LOWER_LEG, RIGHT_LOWER_LEG);

	// Ground
	auto groundId = loadCubeObject(ground, groundS, TEX_GROUND);
	// rotate the ground so it is on the same orientation as the camera
	_gameObjects[groundId]._transform.rotation = {0, M_PI, 0};

	// Skybox
	auto skyboxId = loadCubeObject({0.f}, {100.f}, TEX_SKYBOX);
	_skybox = &_gameObjects[skyboxId];
	_skybox->_transform.rotation = {0, M_PI, 0};
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
		.setMaxSets(MAX_FRAMES * 2 + 1)
		.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES)
		.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES)
		.addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES)
		.build();
	if (scene == HUMANGL) loadHumanGL();
	else if (scene == SCOP) loadScop();
}

MainClass::~MainClass(){
}

#define COPY_TEX(storage, texId, texName)  do {\
		storage.texUv[TEXID(texId, Front)] =  idMap.texName##Front; \
		storage.texUv[TEXID(texId, Top)] =    idMap.texName##Top; \
		storage.texUv[TEXID(texId, Bottom)] = idMap.texName##Bottom; \
		storage.texUv[TEXID(texId, Left)] =   idMap.texName##Left; \
		storage.texUv[TEXID(texId, Right)] =  idMap.texName##Right; \
		storage.texUv[TEXID(texId, Back)] =   idMap.texName##Back; \
	} while (0);

void MainClass::setTextureUvs(TexIdData &texIdBuf)
{
	for (auto &uv : texIdBuf.texUv)
		uv = {{0.f, 0.f}, {1.f, 1.f}   };
	COPY_TEX(texIdBuf, TEX_SKYBOX, skybox);
	COPY_TEX(texIdBuf, TEX_HEAD,   mcHead);
	COPY_TEX(texIdBuf, TEX_TORSO,  mcTorso);
	
	COPY_TEX(texIdBuf, TEX_LEFT_UPPER_ARM,  mcLeftArmUpper);
	COPY_TEX(texIdBuf, TEX_LEFT_UPPER_LEG,  mcLeftLegUpper);
	COPY_TEX(texIdBuf, TEX_LEFT_LOWER_ARM,  mcLeftArmLower);
	COPY_TEX(texIdBuf, TEX_LEFT_LOWER_LEG,  mcLeftLegLower);
	
	COPY_TEX(texIdBuf, TEX_RIGHT_UPPER_ARM,  mcRightArmUpper);
	COPY_TEX(texIdBuf, TEX_RIGHT_UPPER_LEG,  mcRightLegUpper);
	COPY_TEX(texIdBuf, TEX_RIGHT_LOWER_ARM,  mcRightArmLower);
	COPY_TEX(texIdBuf, TEX_RIGHT_LOWER_LEG,  mcRightLegLower);

	for (uint i = 0; i < 8; i++) {
		texIdBuf.texUv[TEXID(TEX_GROUND, i)]  = idMap.lolin;
		texIdBuf.texUv[TEXID(TEX_HAT_BASE, i)] = idMap.maix;
		texIdBuf.texUv[TEXID(TEX_HAT_RIM, i)]  = idMap.wood;
	}
}

void	MainClass::run(void){
	vector<unique_ptr<VeBuffer>>	uboBuffers(MAX_FRAMES);
	unique_ptr<VeBuffer>			texIdBuffer;
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
	
	texIdBuffer = make_unique<VeBuffer>(
			_veDevice,
			sizeof(TexIdData),
			1,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			_veDevice._properties.limits.minUniformBufferOffsetAlignment
		);
	texIdBuffer->map();


	auto	globalSetLayout = VeDescriptorSetLayout::Builder(_veDevice)
		.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			VK_SHADER_STAGE_ALL_GRAPHICS)
		.addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 
			VK_SHADER_STAGE_FRAGMENT_BIT)
		.addBinding(2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			VK_SHADER_STAGE_ALL_GRAPHICS)
	.build();
	vector<VkDescriptorSet>	globalDescriptorSets(MAX_FRAMES);
	for (int i = 0; i < globalDescriptorSets.size(); i++){
		VkDescriptorBufferInfo	buffer = uboBuffers[i]->descriptorBufferInfo();
		VkDescriptorBufferInfo	texIdBufferDesc = texIdBuffer->descriptorBufferInfo();
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
			.writeBuffer(2, &texIdBufferDesc)
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

	SkyboxRender	skyboxRenderSystem{
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
	
	if (_skybox != nullptr)
		simpleRenderSystem.addIgnoredObject(_skybox->getId());
	TexIdData texIdData{};
	MainClass::setTextureUvs(texIdData);
	texIdBuffer->writeToBuffer(&texIdData);
	texIdBuffer->flush();

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
			if (_skybox != nullptr)
				skyboxRenderSystem.renderSkybox(frameInfo, *_skybox);
			_veRenderer.endSwapChainRenderPass(commandBuffer);
			_veRenderer.endFrame();
		}
	}
	vkDeviceWaitIdle(_veDevice.device());
}
