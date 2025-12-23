#pragma once

#include <VeWindow.hpp>

using namespace std;
using LayerList = vector<const char*>;

struct SwapChainSupportDetails{
	VkSurfaceCapabilitiesKHR	capabilities;
	vector<VkSurfaceFormatKHR>	formats;
	vector<VkPresentModeKHR>	presentModes;
};

struct QueueFamilyIndices{
	uint	graphicsFamily;
	uint	presentFamily;
	bool	graphicsFamilyHasValue = false;
	bool	presentFamilyHasValue = false;

	bool	isComplete(void);
};

class VeDevice{
private:
	VkInstance			_instance;
	VkPhysicalDevice	_physicalDevice = VK_NULL_HANDLE;
	VeWindow			&_window;
	VkCommandPool		_commandPool;
	VkDevice			_device;
	VkSurfaceKHR		_surface;
	VkQueue				_graphicsQueue;
	VkQueue				_presentQueue;
	const LayerList		_validationLayers = {"VK_LAYER_KHRONOS_validation"};
	const LayerList		_deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	VkDebugUtilsMessengerEXT	_debugMessenger;

	void	createInstance(void);
	void	setupDebugMessenger(void);
	void	createSurface(void);
	void	pickPhysicalDevice(void);
	void	createLogicalDevice(void);
	void	createCommandPool(void);

	// Helper functions
	bool	isDeviceSuitable(VkPhysicalDevice device);
	bool	checkValidationLayerSupport(void);
	void	populateDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT &info);
	bool	checkDeviceExtensionSupport(VkPhysicalDevice device);
	LayerList				getRequiredExtensions(void);
	QueueFamilyIndices		findQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails	querySwapChainSupport(VkPhysicalDevice device);
public:
	const bool 					_enableValidationLayers = true;
	VkPhysicalDeviceProperties	_properties;

	VeDevice(VeWindow &window);
	~VeDevice(void);

	// Getter
	VkCommandPool			getCommandPool(void);
	VkDevice				device(void);
	VkSurfaceKHR			surface(void);
	VkQueue					graphicsQueue(void);
	VkQueue					presentQueue(void);
	SwapChainSupportDetails	getSwapChainSupport(void);

	// Find functions
	uint				findMemoryType(uint, VkMemoryPropertyFlags);
	QueueFamilyIndices	findPhysicalQueueFamilies(void);
	VkFormat			findSupportedFormat(
		const vector<VkFormat>&, VkImageTiling, VkFormatFeatureFlags
	);

	// Buffer helper functions
	void			createBuffer(
		VkDeviceSize size,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkBuffer &buffer,
		VkDeviceMemory &bufferMemory
	);
	VkCommandBuffer	beginSingleTimeCommands(void);
	void	endSingleTimeCommands(VkCommandBuffer commandBuffer);
	void	copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size);
	void	copyBufferToImage(VkBuffer, VkImage, uint, uint, uint);
	void	createImageWithInfo(
		const VkImageCreateInfo &imageInfo,
		VkMemoryPropertyFlags properties,
		VkImage &image,
		VkDeviceMemory &imageMemory
	);
};
