#pragma once

#include <VeDevice.hpp>

using Binding = unordered_map<uint32_t, VkDescriptorSetLayoutBinding>;

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
					uint32_t binding,
					VkDescriptorType descriptorType,
					VkShaderStageFlags stageFlags,
					uint32_t count = 1
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
				uint32_t						_maxSets = 1000;
				VkDescriptorPoolCreateFlags		_poolFlags = 0;
			public:
				Builder(VeDevice &device);

				Builder	&addPoolSize(VkDescriptorType descriptorType, uint32_t count);
				Builder	&setPoolFlags(VkDescriptorPoolCreateFlags flags);
				Builder	&setMaxSets(uint32_t count);
				unique_ptr<VeDescriptorPool>	build() const;
		};

		VeDescriptorPool(
			VeDevice &device,
			uint32_t maxSets,
			VkDescriptorPoolCreateFlags poolFlags,
			const vector<VkDescriptorPoolSize> &poolSizes
		);
		~VeDescriptorPool();

		bool	allocateDescriptor(
			const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor
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
		VeDescriptorWriter(VeDescriptorSetLayout &setLayout, VeDescriptorPool &pool);

		VeDescriptorWriter	&writeBuffer(uint32_t bind, VkDescriptorBufferInfo *buffer);
		VeDescriptorWriter	&writeImage(uint32_t bind, VkDescriptorImageInfo *image);

		bool	build(VkDescriptorSet &set);
		void	overwrite(VkDescriptorSet &set);
};
