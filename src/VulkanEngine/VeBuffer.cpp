#include <VeBuffer.hpp>

VkDeviceSize	VeBuffer::getAlignment(VkDeviceSize deviceSize, VkDeviceSize minOffsetAlignment){
	if (minOffsetAlignment > 0)
		return ((deviceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1));
	return (deviceSize);
}

VeBuffer::VeBuffer(
	VeDevice &device,
	VkDeviceSize instanceSize,
	uint32_t instanceCount,
	VkBufferUsageFlags usageFlags,
	VkMemoryPropertyFlags memoryPropertyFlags,
	VkDeviceSize minOffsetAlignment
):
	_veDevice(device),
	_instanceSize(instanceSize),
	_instanceCount(instanceCount),
	_usageFlags(usageFlags),
	_memoryPropertyFlags(memoryPropertyFlags)
{
	_alignmentSize = getAlignment(_instanceSize, minOffsetAlignment);
	_bufferSize = _alignmentSize * _instanceCount;
	device.createBuffer(_bufferSize, _usageFlags, _memoryPropertyFlags, _buffer, _memory);
}

VeBuffer::~VeBuffer(){
	unmap();
	vkDestroyBuffer(_veDevice.device(), _buffer, nullptr);
	vkFreeMemory(_veDevice.device(), _memory, nullptr);
}

VkResult VeBuffer::map(VkDeviceSize size, VkDeviceSize offset){
	return (vkMapMemory(_veDevice.device(), _memory, offset, size, 0, &_mapped));
}

void VeBuffer::unmap(){
	if (_mapped){
		vkUnmapMemory(_veDevice.device(), _memory);
		_mapped = nullptr;
	}
}

void VeBuffer::writeToBuffer(void *data, VkDeviceSize size, VkDeviceSize offset){
	if (size == VK_WHOLE_SIZE){
		memcpy(_mapped, data, _bufferSize);
	} else {
		char	*memOffset = (char *)_mapped;
		memOffset += offset;
		memcpy(memOffset, data, size);
	}
}

VkResult VeBuffer::flush(VkDeviceSize size, VkDeviceSize offset){
	VkMappedMemoryRange	mappedRange{};
	mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	mappedRange.memory = _memory;
	mappedRange.offset = offset;
	mappedRange.size = size;

	return (vkFlushMappedMemoryRanges(_veDevice.device(), 1, &mappedRange));
}

VkDescriptorBufferInfo VeBuffer::descriptorBufferInfo(VkDeviceSize size, VkDeviceSize offset){
	return (VkDescriptorBufferInfo{_buffer, offset, size});
}

VkBuffer				VeBuffer::getBuffer(void) const {
	return(_buffer);
}
