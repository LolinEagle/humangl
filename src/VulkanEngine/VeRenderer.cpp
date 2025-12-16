#include <VeRenderer.hpp>

void	VeRenderer::createCommandBuffers(void){
	_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo	allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _veDevice.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

	if (vkAllocateCommandBuffers(_veDevice.device(), &allocInfo, _commandBuffers.data()) != 0)
		throw (runtime_error("failed to allocate command buffers"));
}

void	VeRenderer::freeCommandBuffers(void){
	vkFreeCommandBuffers(
		_veDevice.device(),
		_veDevice.getCommandPool(),
		static_cast<uint32_t>(_commandBuffers.size()),
		_commandBuffers.data()
	);
	_commandBuffers.clear();
}

void	VeRenderer::recreateSwapChain(void){
	auto	extent = _veWindow.getExtent();

	while (extent.width == 0 || extent.height == 0){
		extent = _veWindow.getExtent();
		glfwWaitEvents();
	}
	vkDeviceWaitIdle(_veDevice.device());
	if (_veSwapChain == nullptr){
		_veSwapChain = make_unique<VeSwapChain>(_veDevice, extent);
	} else {
		shared_ptr<VeSwapChain>	oldSwapChain = move(_veSwapChain);
		_veSwapChain = make_unique<VeSwapChain>(_veDevice, extent, oldSwapChain);
		if (!oldSwapChain->compareSwapFormate(*_veSwapChain.get()))
			throw (runtime_error("swap chain image (or depth) format has changed"));
	}
}

VeRenderer::VeRenderer(VeWindow &veWindow, VeDevice &veDevice)
: _veWindow(veWindow), _veDevice(veDevice){
	recreateSwapChain();
	createCommandBuffers();
}

VeRenderer::~VeRenderer(){
	freeCommandBuffers();
}

int				VeRenderer::getCurrentFrameIndex(void) const {
	return (_currentFrameIndex);
};

VkCommandBuffer	VeRenderer::beginFrame(void){
	auto	result = _veSwapChain->acquireNextImage(&_currentImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR){
		recreateSwapChain();
		return (nullptr);
	}
	if (result != 0 && result != VK_SUBOPTIMAL_KHR)
		throw (runtime_error("failed to acquire swap chain image"));
	_isFrameStarted = true;

	auto	commandBuffer = getCurrentCommandBuffer();

	VkCommandBufferBeginInfo	beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != 0)
		throw (runtime_error("failed to begin recording command buffer"));
	return (commandBuffer);
}

void			VeRenderer::endFrame(void){
	auto	commandBuffer = getCurrentCommandBuffer();

	if (vkEndCommandBuffer(commandBuffer) != 0)
		throw (runtime_error("failed to record command buffer"));

	auto	result = _veSwapChain->submitCommandBuffers(&commandBuffer, &_currentImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR ||
		result == VK_SUBOPTIMAL_KHR ||
		_veWindow.wasWindowResized()
	){
		_veWindow.resetWindowResizedFlag();
		recreateSwapChain();
	} else if (result != 0)
		throw (runtime_error("failed to present swap chain image"));
	_isFrameStarted = false;
	_currentFrameIndex = (_currentFrameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
}

void			VeRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer){
	array<VkClearValue, 2>	clearValues{};
	clearValues[0].color = {0.01, 0.01, 0.01, 1};
	clearValues[1].depthStencil = {1.0f, 0};

	VkRenderPassBeginInfo	renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = _veSwapChain->getRenderPass();
	renderPassInfo.framebuffer = _veSwapChain->getFrameBuffer(_currentImageIndex);
	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = _veSwapChain->getSwapChainExtent();
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport	viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(_veSwapChain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(_veSwapChain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D	scissor{{0, 0}, _veSwapChain->getSwapChainExtent()};

	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void			VeRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer){
	vkCmdEndRenderPass(commandBuffer);
}

VkRenderPass	VeRenderer::getSwapchainRenderPass(void) const {
	return (_veSwapChain->getRenderPass());
}

float			VeRenderer::getAspectRatio(void) const {
	return (_veSwapChain->extentAspectRatio());
}

VkCommandBuffer	VeRenderer::getCurrentCommandBuffer(void) const {
	return (_commandBuffers[_currentFrameIndex]);
}
