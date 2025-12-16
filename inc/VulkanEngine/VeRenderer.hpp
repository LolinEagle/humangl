#pragma once

#include <VeDevice.hpp>
#include <VeModel.hpp>
#include <VeSwapChain.hpp>
#include <VeWindow.hpp>

using namespace std;

class VeRenderer{
	private:
		VeWindow				&_veWindow;
		VeDevice				&_veDevice;
		unique_ptr<VeSwapChain>	_veSwapChain;
		vector<VkCommandBuffer>	_commandBuffers;

		void	createCommandBuffers(void);
		void	freeCommandBuffers(void);
		void	recreateSwapChain(void);
	public:
		uint32_t	_currentImageIndex;
		int			_currentFrameIndex = 0;
		bool		_isFrameStarted;

		VeRenderer(VeWindow &veWindow, VeDevice &veDevice);
		~VeRenderer();

		int				getCurrentFrameIndex(void) const;
		VkCommandBuffer	beginFrame(void);
		void			endFrame(void);
		void			beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void			endSwapChainRenderPass(VkCommandBuffer commandBuffer);
		VkRenderPass	getSwapchainRenderPass(void) const;
		float			getAspectRatio(void) const;
		VkCommandBuffer	getCurrentCommandBuffer(void) const;
};
