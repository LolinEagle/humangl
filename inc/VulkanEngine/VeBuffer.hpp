#pragma once

#include <VeDevice.hpp>

#define VE_SIZE VkDeviceSize size = VK_WHOLE_SIZE
#define VE_OFFSET VkDeviceSize offset = 0

class VeBuffer{
private:
	VeDevice				&_veDevice;
	void					*_mapped = nullptr;
	VkBuffer				_buffer = VK_NULL_HANDLE;
	VkDeviceMemory			_memory = VK_NULL_HANDLE;
	VkDeviceSize			_bufferSize;
	uint					_instanceCount;
	VkDeviceSize			_instanceSize;
	VkDeviceSize			_alignmentSize;
	VkBufferUsageFlags		_usageFlags;
	VkMemoryPropertyFlags	_memoryPropertyFlags;

	static VkDeviceSize	getAlignment(VkDeviceSize, VkDeviceSize);
public:
	VeBuffer(
		VeDevice &device,
		VkDeviceSize instanceSize,
		uint instanceCount,
		VkBufferUsageFlags usageFlags,
		VkMemoryPropertyFlags memoryPropertyFlags,
		VkDeviceSize minOffsetAlignment = 1
	);
	~VeBuffer();

	VkResult				map(VE_SIZE, VE_OFFSET);
	void					unmap(void);
	void					writeToBuffer(void *data, VE_SIZE, VE_OFFSET);
	VkResult				flush(VE_SIZE, VE_OFFSET);
	VkDescriptorBufferInfo	descriptorBufferInfo(VE_SIZE, VE_OFFSET);
	VkBuffer				getBuffer(void) const;
};
