#pragma once

#include <VeDevice.hpp>

using namespace std;

class VeSwapChain{
	private:
		VkFormat				_swapChainImageFormat;
		VkFormat				_swapChainDepthFormat;
		VkExtent2D				_swapChainExtent;
		vector<VkFramebuffer>	_swapChainFramebuffers;
		VkRenderPass			_renderPass;
		vector<VkImage>			_depthImages;
		vector<VkDeviceMemory>	_depthImageMemorys;
		vector<VkImageView>		_depthImageViews;
		vector<VkImage>			_swapChainImages;
		vector<VkImageView>		_swapChainImageViews;
		VeDevice				&_device;
		VkExtent2D				_windowExtent;
		VkSwapchainKHR			_swapChain;
		shared_ptr<VeSwapChain>	_oldSwapChain;
		vector<VkSemaphore>		_imageAvailableSemaphores;
		vector<VkSemaphore>		_renderFinishedSemaphores;
		vector<VkFence>			_inFlightFences;
		vector<VkFence>			_imagesInFlight;
		size_t					_currentFrame = 0;

		void	init(void);
		void	createSwapChain(void);
		void	createImageViews(void);
		void	createDepthResources(void);
		void	createRenderPass(void);
		void	createFramebuffers(void);
		void	createSyncObjects(void);

		// Helper functions
		VkSurfaceFormatKHR	chooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR> &format);
		VkPresentModeKHR	chooseSwapPresentMode(const vector<VkPresentModeKHR> &present);
		VkExtent2D			chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
	public:
		VeSwapChain(VeDevice &deviceRef, VkExtent2D extent);
		VeSwapChain(VeDevice &deviceRef, VkExtent2D extent, shared_ptr<VeSwapChain> previous);
		~VeSwapChain();

		VkFramebuffer	getFrameBuffer(int index);
		VkRenderPass	getRenderPass(void);
		VkImageView		getImageView(int index);
		size_t			imageCount(void);
		VkFormat		getSwapChainImageFormat(void);
		VkExtent2D		getSwapChainExtent(void);
		uint32_t		width(void);
		uint32_t		height(void);
		float			extentAspectRatio(void);
		VkFormat		findDepthFormat(void);
		VkResult		acquireNextImage(uint32_t *imageIndex);
		VkResult		submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *image);
		bool			compareSwapFormate(const VeSwapChain &swapChain) const;
};
