#pragma once

#include <VeDevice.hpp>

#define VE_BUFFER_DEFAULT_ARGUMENT VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0

class VeBuffer{
	private:
		VeDevice				&_veDevice;
		void					*_mapped = nullptr;
		VkBuffer				_buffer = VK_NULL_HANDLE;
		VkDeviceMemory			_memory = VK_NULL_HANDLE;
		VkDeviceSize			_bufferSize;
		uint32_t				_instanceCount;
		VkDeviceSize			_instanceSize;
		VkDeviceSize			_alignmentSize;
		VkBufferUsageFlags		_usageFlags;
		VkMemoryPropertyFlags	_memoryPropertyFlags;

		static VkDeviceSize	getAlignment(VkDeviceSize deviceSize, VkDeviceSize minOffsetAlignment);
	public:
		VeBuffer(
			VeDevice &device,
			VkDeviceSize instanceSize,
			uint32_t instanceCount,
			VkBufferUsageFlags usageFlags,
			VkMemoryPropertyFlags memoryPropertyFlags,
			VkDeviceSize minOffsetAlignment = 1
		);
		~VeBuffer();

		VkResult				map(VE_BUFFER_DEFAULT_ARGUMENT);
		void					unmap(void);
		void					writeToBuffer(void *data, VE_BUFFER_DEFAULT_ARGUMENT);
		VkResult				flush(VE_BUFFER_DEFAULT_ARGUMENT);
		VkDescriptorBufferInfo	descriptorBufferInfo(VE_BUFFER_DEFAULT_ARGUMENT);
		VkBuffer				getBuffer(void) const;
};
