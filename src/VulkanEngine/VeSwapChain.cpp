#include <VeSwapChain.hpp>

using namespace std;

void	VeSwapChain::init(void){
	createSwapChain();
	createImageViews();
	createRenderPass();
	createDepthResources();
	createFramebuffers();
	createSyncObjects();
}

void	VeSwapChain::createSwapChain(void){
	SwapChainSupportDetails	swapChainSupport = _device.getSwapChainSupport();
	VkSurfaceFormatKHR		surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR		presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D				extent = chooseSwapExtent(swapChainSupport.capabilities);
	uint32_t				imageCount = swapChainSupport.capabilities.minImageCount + 1;
	const uint32_t			maxImageCount = swapChainSupport.capabilities.maxImageCount;

	if (maxImageCount > 0 && imageCount > maxImageCount)
		imageCount = maxImageCount;

	VkSwapchainCreateInfoKHR	createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = _device.surface();
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices	indices = _device.findPhysicalQueueFamilies();
	uint32_t			queueFamilyIndices[] = {indices.graphicsFamily, indices.presentFamily};

	if (indices.graphicsFamily != indices.presentFamily){
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = _oldSwapChain == nullptr ? VK_NULL_HANDLE : _oldSwapChain->_swapChain;

	if (vkCreateSwapchainKHR(_device.device(), &createInfo, nullptr, &_swapChain) != 0)
		throw (runtime_error("failed to create swap chain"));

	// We only specified a minimum number of images in the swap chain, so the implementation is
	// allowed to create a swap chain with more. That's why we'll first query the final number of
	// images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
	// retrieve the handles.
	vkGetSwapchainImagesKHR(_device.device(), _swapChain, &imageCount, nullptr);
	_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(_device.device(), _swapChain, &imageCount, _swapChainImages.data());
	_swapChainImageFormat = surfaceFormat.format;
	_swapChainExtent = extent;
}

void	VeSwapChain::createImageViews(void){
	_swapChainImageViews.resize(_swapChainImages.size());
	for (size_t i = 0; i < _swapChainImages.size(); i++){
		VkImageViewCreateInfo	viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = _swapChainImages[i];
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = _swapChainImageFormat;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(_device.device(), &viewInfo, nullptr, &_swapChainImageViews[i]) != 0)
			throw (runtime_error("failed to create texture image view"));
	}
}

void	VeSwapChain::createDepthResources(void){
	VkFormat	depthFormat = findDepthFormat();
	VkExtent2D	swapChainExtent = getSwapChainExtent();

	_swapChainDepthFormat = depthFormat;
	_depthImages.resize(imageCount());
	_depthImageMemorys.resize(imageCount());
	_depthImageViews.resize(imageCount());
	for (int i = 0; i < _depthImages.size(); i++){
		VkImageCreateInfo	imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = swapChainExtent.width;
		imageInfo.extent.height = swapChainExtent.height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = depthFormat;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = 0;

		_device.createImageWithInfo(
			imageInfo,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			_depthImages[i],
			_depthImageMemorys[i]
		);

		VkImageViewCreateInfo	viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = _depthImages[i];
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = depthFormat;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(_device.device(), &viewInfo, nullptr, &_depthImageViews[i]) != 0)
			throw (runtime_error("failed to create texture image view"));
	}
}

void	VeSwapChain::createRenderPass(void){
	VkAttachmentDescription	depthAttachment{};
	depthAttachment.format = findDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference	depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription	colorAttachment{};
	colorAttachment.format = getSwapChainImageFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference	colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription	subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency	dependency{};
	dependency.dstSubpass = 0;
	dependency.dstAccessMask =
		VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependency.dstStageMask =
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.srcAccessMask = 0;
	dependency.srcStageMask =
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

	array<VkAttachmentDescription, 2>	attachments = {colorAttachment, depthAttachment};

	VkRenderPassCreateInfo	renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(_device.device(), &renderPassInfo, nullptr, &_renderPass) != 0)
		throw (runtime_error("failed to create render pass"));
}

void	VeSwapChain::createFramebuffers(void){
	_swapChainFramebuffers.resize(imageCount());
	for (size_t i = 0; i < imageCount(); i++){
		array<VkImageView, 2>	attachments = {_swapChainImageViews[i], _depthImageViews[i]};
		VkExtent2D				swapChainExtent = getSwapChainExtent();

		VkFramebufferCreateInfo	framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = _renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(
			_device.device(),
			&framebufferInfo,
			nullptr,
			&_swapChainFramebuffers[i]) != 0
		)
			throw (runtime_error("failed to create framebuffer"));
	}
}

void	VeSwapChain::createSyncObjects(void){
	_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	_imagesInFlight.resize(imageCount(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo	si{};
	si.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo	fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
		if (
			vkCreateSemaphore(_device.device(), &si, nullptr, &_imageAvailableSemaphores[i]) != 0 ||
			vkCreateSemaphore(_device.device(), &si, nullptr, &_renderFinishedSemaphores[i]) != 0 ||
			vkCreateFence(_device.device(), &fenceInfo, nullptr, &_inFlightFences[i]) != 0
		)
			throw (runtime_error("failed to create synchronization objects for a frame"));
	}
}

VkSurfaceFormatKHR	VeSwapChain::chooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR> &format){
	for (const auto &availableFormat : format){
		if (
			availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
		)
			return (availableFormat);
	}
	return (format[0]);
}

VkPresentModeKHR	VeSwapChain::chooseSwapPresentMode(const vector<VkPresentModeKHR> &present){
	// Present mode: Mailbox
	const auto	&presentMode = VK_PRESENT_MODE_MAILBOX_KHR;

	// Present mode: Immediate	
	// const auto	&presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;

	// Find if present mode is available
	for (const auto	&availablePresentMode : present){
		if (availablePresentMode == presentMode)
			return (availablePresentMode);
	}

	// Present mode: V-Sync
	return (VK_PRESENT_MODE_FIFO_KHR);
}

VkExtent2D			VeSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities){
	if (capabilities.currentExtent.width != numeric_limits<uint32_t>::max()){
		return (capabilities.currentExtent);
	} else {
		VkExtent2D	actualExtent = _windowExtent;
		actualExtent.width = max(
			capabilities.minImageExtent.width,
			min(capabilities.maxImageExtent.width, actualExtent.width)
		);
		actualExtent.height = max(
			capabilities.minImageExtent.height,
			min(capabilities.maxImageExtent.height, actualExtent.height)
		);
		return (actualExtent);
	}
}

VeSwapChain::VeSwapChain(VeDevice &deviceRef, VkExtent2D extent)
: _device(deviceRef), _windowExtent(extent){
	init();
}

VeSwapChain::VeSwapChain(VeDevice &deviceRef, VkExtent2D extent, shared_ptr<VeSwapChain> previous)
: _device(deviceRef), _windowExtent(extent), _oldSwapChain(previous){
	init();
	_oldSwapChain = nullptr;
}

VeSwapChain::~VeSwapChain(){
	for (auto imageView : _swapChainImageViews)
		vkDestroyImageView(_device.device(), imageView, nullptr);
	_swapChainImageViews.clear();
	if (_swapChain != nullptr){
		vkDestroySwapchainKHR(_device.device(), _swapChain, nullptr);
		_swapChain = nullptr;
	}
	for (int i = 0; i < _depthImages.size(); i++){
		vkDestroyImageView(_device.device(), _depthImageViews[i], nullptr);
		vkDestroyImage(_device.device(), _depthImages[i], nullptr);
		vkFreeMemory(_device.device(), _depthImageMemorys[i], nullptr);
	}
	for (auto framebuffer : _swapChainFramebuffers)
		vkDestroyFramebuffer(_device.device(), framebuffer, nullptr);
	vkDestroyRenderPass(_device.device(), _renderPass, nullptr);

	// Cleanup synchronization objects
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
		vkDestroySemaphore(_device.device(), _renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(_device.device(), _imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(_device.device(), _inFlightFences[i], nullptr);
	}
}

VkFramebuffer	VeSwapChain::getFrameBuffer(int index){
	return (_swapChainFramebuffers[index]);
}

VkRenderPass	VeSwapChain::getRenderPass(void){
	return (_renderPass);
}

VkImageView		VeSwapChain::getImageView(int index){
	return (_swapChainImageViews[index]);
}

size_t			VeSwapChain::imageCount(void){
	return (_swapChainImages.size());
}

VkFormat		VeSwapChain::getSwapChainImageFormat(void){
	return (_swapChainImageFormat);
}

VkExtent2D		VeSwapChain::getSwapChainExtent(void){
	return (_swapChainExtent);
}

uint32_t		VeSwapChain::width(void){
	return (_swapChainExtent.width);
}

uint32_t		VeSwapChain::height(void){
	return (_swapChainExtent.height);
}

float			VeSwapChain::extentAspectRatio(void){
	return (static_cast<float>(_swapChainExtent.width) / static_cast<float>(_swapChainExtent.height));
}

VkFormat		VeSwapChain::findDepthFormat(void){
	return (_device.findSupportedFormat(
		{VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	));
}

VkResult		VeSwapChain::acquireNextImage(uint32_t *imageIndex){
	vkWaitForFences(
		_device.device(),
		1,
		&_inFlightFences[_currentFrame],
		VK_TRUE,
		numeric_limits<uint64_t>::max()
	);

	VkResult	result = vkAcquireNextImageKHR(
		_device.device(),
		_swapChain,
		numeric_limits<uint64_t>::max(),
		_imageAvailableSemaphores[_currentFrame],// Must be a not signaled semaphore
		VK_NULL_HANDLE,
		imageIndex
	);

	return (result);
}

VkResult		VeSwapChain::submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *image){
	if (_imagesInFlight[*image] != VK_NULL_HANDLE) 
		vkWaitForFences(_device.device(), 1, &_imagesInFlight[*image], VK_TRUE, UINT64_MAX);
	_imagesInFlight[*image] = _inFlightFences[_currentFrame];

	VkSubmitInfo	submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore				waitSemaphores[] = {_imageAvailableSemaphores[_currentFrame]};
	VkPipelineStageFlags	waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = buffers;

	VkSemaphore	signalSemaphores[] = {_renderFinishedSemaphores[_currentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(_device.device(), 1, &_inFlightFences[_currentFrame]);
	if (vkQueueSubmit(_device.graphicsQueue(), 1, &submitInfo, _inFlightFences[_currentFrame]) != 0)
		throw (runtime_error("failed to submit draw command buffer"));

	VkPresentInfoKHR	presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR	swapChains[] = {_swapChain};

	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = image;

	auto	result = vkQueuePresentKHR(_device.presentQueue(), &presentInfo);

	_currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	return (result);
}

bool			VeSwapChain::compareSwapFormate(const VeSwapChain &swapChain) const {
	return (swapChain._swapChainDepthFormat == _swapChainDepthFormat &&
			swapChain._swapChainImageFormat == _swapChainImageFormat);
}
