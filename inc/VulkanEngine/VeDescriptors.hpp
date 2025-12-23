#pragma once

#include <VeDevice.hpp>

using Binding = unordered_map<uint, VkDescriptorSetLayoutBinding>;

class VeDescriptorSetLayout{
private:
	VeDevice				&_device;
	VkDescriptorSetLayout	_descriptorSetLayout;
	Binding					_bindings;

	friend class VeDescriptorWriter;
public:
	class Builder{
	private:
		VeDevice	&_device;
		Binding		_bindings{};
	public:
		Builder(VeDevice &device);

		Builder	&addBinding(
			uint binding,
			VkDescriptorType descriptorType,
			VkShaderStageFlags stageFlags,
			uint count = 1
		);
		unique_ptr<VeDescriptorSetLayout> build() const;
	};

	VeDescriptorSetLayout(VeDevice &device, Binding bindings);
	~VeDescriptorSetLayout();

	VkDescriptorSetLayout	getDescriptorSetLayout(void) const;
};

class VeDescriptorPool{
private:
	VeDevice			&_device;
	VkDescriptorPool	_descriptorPool;

	friend class VeDescriptorWriter;
public:
	class Builder{
	private:
		VeDevice						&_device;
		vector<VkDescriptorPoolSize>	_poolSizes{};
		uint							_maxSets = 1000;
		VkDescriptorPoolCreateFlags		_poolFlags = 0;
	public:
		Builder(VeDevice &device);

		Builder	&addPoolSize(VkDescriptorType descriptorType, uint count);
		Builder	&setPoolFlags(VkDescriptorPoolCreateFlags flags);
		Builder	&setMaxSets(uint count);
		unique_ptr<VeDescriptorPool>	build() const;
	};

	VeDescriptorPool(
		VeDevice &device,
		uint maxSets,
		VkDescriptorPoolCreateFlags poolFlags,
		const vector<VkDescriptorPoolSize> &poolSizes
	);
	~VeDescriptorPool();

	bool	allocateDescriptor(
		const VkDescriptorSetLayout descriptorSetLayout,
		VkDescriptorSet &descriptor
	) const;
	void	freeDescriptors(vector<VkDescriptorSet> &descriptors) const;
	void	resetPool(void);
};

class VeDescriptorWriter{
private:
	VeDescriptorSetLayout			&_setLayout;
	VeDescriptorPool				&_pool;
	vector<VkWriteDescriptorSet>	_writes;
public:
	VeDescriptorWriter(VeDescriptorSetLayout&, VeDescriptorPool&);

	VeDescriptorWriter	&writeBuffer(uint bind, VkDescriptorBufferInfo *buffer);
	VeDescriptorWriter	&writeImage(uint bind, VkDescriptorImageInfo *image);

	bool	build(VkDescriptorSet &set);
	void	overwrite(VkDescriptorSet &set);
};
