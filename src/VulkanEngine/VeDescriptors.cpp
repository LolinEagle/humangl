#include <VeDescriptors.hpp>

/* VeDescriptorSetLayout **************************************************** */

VeDescriptorSetLayout::Builder::Builder(VeDevice &device) : _device(device) {}

VeDescriptorSetLayout::Builder &VeDescriptorSetLayout::Builder::addBinding(
	uint32_t binding, VkDescriptorType descriptorType,
	VkShaderStageFlags stageFlags, uint32_t count) {
	VkDescriptorSetLayoutBinding layoutBinding{};
	layoutBinding.binding = binding;
	layoutBinding.descriptorCount = count;
	layoutBinding.descriptorType = descriptorType;
	layoutBinding.pImmutableSamplers = nullptr;
	layoutBinding.stageFlags = stageFlags;

	_bindings[binding] = layoutBinding;
	return (*this);
}

unique_ptr<VeDescriptorSetLayout> VeDescriptorSetLayout::Builder::build()
	const {
	return (make_unique<VeDescriptorSetLayout>(_device, _bindings));
}

VeDescriptorSetLayout::VeDescriptorSetLayout(VeDevice &device, Binding bindings)
	: _device(device), _bindings(bindings) {
	vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
	for (auto kv : bindings) setLayoutBindings.push_back(kv.second);

	VkDescriptorSetLayoutCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	info.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
	info.pBindings = setLayoutBindings.data();

	if (vkCreateDescriptorSetLayout(_device.device(), &info, nullptr,
									&_descriptorSetLayout) != 0)
		throw(runtime_error("failed to create descriptor set layout"));
}

VeDescriptorSetLayout::~VeDescriptorSetLayout() {
	vkDestroyDescriptorSetLayout(_device.device(), _descriptorSetLayout,
								 nullptr);
}

VkDescriptorSetLayout VeDescriptorSetLayout::getDescriptorSetLayout(
	void) const {
	return (_descriptorSetLayout);
}

/* VeDescriptorPool ********************************************************* */

VeDescriptorPool::Builder::Builder(VeDevice &device) : _device(device) {}

VeDescriptorPool::Builder &VeDescriptorPool::Builder::addPoolSize(
	VkDescriptorType descriptorType, uint32_t count) {
	_poolSizes.push_back({descriptorType, count});
	return (*this);
}

VeDescriptorPool::Builder &VeDescriptorPool::Builder::setPoolFlags(
	VkDescriptorPoolCreateFlags flags) {
	_poolFlags = flags;
	return (*this);
}

VeDescriptorPool::Builder &VeDescriptorPool::Builder::setMaxSets(
	uint32_t count) {
	_maxSets = count;
	return (*this);
}

unique_ptr<VeDescriptorPool> VeDescriptorPool::Builder::build() const {
	return (make_unique<VeDescriptorPool>(_device, _maxSets, _poolFlags,
										  _poolSizes));
}

VeDescriptorPool::VeDescriptorPool(
	VeDevice &device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags,
	const vector<VkDescriptorPoolSize> &poolSizes)
	: _device(device) {
	VkDescriptorPoolCreateInfo poolCreateInfo{};
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolCreateInfo.pPoolSizes = poolSizes.data();
	poolCreateInfo.maxSets = maxSets;
	poolCreateInfo.flags = poolFlags;

	if (vkCreateDescriptorPool(_device.device(), &poolCreateInfo, nullptr,
							   &_descriptorPool) != 0)
		throw(runtime_error("failed to create descriptor pool"));
}

VeDescriptorPool::~VeDescriptorPool() {
	vkDestroyDescriptorPool(_device.device(), _descriptorPool, nullptr);
}

bool VeDescriptorPool::allocateDescriptor(
	const VkDescriptorSetLayout descriptorSetLayout,
	VkDescriptorSet &descriptor) const {
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = _descriptorPool;
	allocInfo.pSetLayouts = &descriptorSetLayout;
	allocInfo.descriptorSetCount = 1;

	// Might want to create a "DescriptorPoolManager" class that handles this
	// case, and builds a new pool whenever an old pool fills up. But this is
	// beyond our current scope
	if (vkAllocateDescriptorSets(_device.device(), &allocInfo, &descriptor) !=
		0)
		return (false);
	return (true);
}

void VeDescriptorPool::freeDescriptors(
	vector<VkDescriptorSet> &descriptors) const {
	vkFreeDescriptorSets(_device.device(), _descriptorPool,
						 static_cast<uint32_t>(descriptors.size()),
						 descriptors.data());
}

void VeDescriptorPool::resetPool(void) {
	vkResetDescriptorPool(_device.device(), _descriptorPool, 0);
}

/* VeDescriptorWriter ******************************************************* */

VeDescriptorWriter::VeDescriptorWriter(VeDescriptorSetLayout &setLayout,
									   VeDescriptorPool &pool)
	: _setLayout(setLayout), _pool(pool) {}

VeDescriptorWriter &VeDescriptorWriter::writeBuffer(
	uint32_t bind, VkDescriptorBufferInfo *buffer) {
	auto &bindingDescription = _setLayout._bindings[bind];

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.descriptorType = bindingDescription.descriptorType;
	write.dstBinding = bind;
	write.pBufferInfo = buffer;
	write.descriptorCount = 1;

	_writes.push_back(write);
	return (*this);
}

VeDescriptorWriter &VeDescriptorWriter::writeImage(
	uint32_t bind, VkDescriptorImageInfo *image) {
	auto &bindingDescription = _setLayout._bindings[bind];

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.descriptorType = bindingDescription.descriptorType;
	write.dstBinding = bind;
	write.pImageInfo = image;
	write.descriptorCount = 1;

	_writes.push_back(write);
	return (*this);
}

bool VeDescriptorWriter::build(VkDescriptorSet &set) {
	bool success =
		_pool.allocateDescriptor(_setLayout.getDescriptorSetLayout(), set);
	if (!success) return (false);
	overwrite(set);
	return (true);
}

void VeDescriptorWriter::overwrite(VkDescriptorSet &set) {
	for (auto &write : _writes) write.dstSet = set;
	vkUpdateDescriptorSets(_pool._device.device(), _writes.size(),
						   _writes.data(), 0, nullptr);
}
