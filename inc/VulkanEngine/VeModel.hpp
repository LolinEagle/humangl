#pragma once

#include <VeDevice.hpp>
#include <VeBuffer.hpp>

using namespace std;

class VeModel{
	public:
		struct Vertex{
			vem::vec3	position, color, normal;
			vem::vec2	uv;

			static vector<VkVertexInputBindingDescription>		getBindingDescriptions(void);
			static vector<VkVertexInputAttributeDescription>	getAttributeDescriptions(void);

			bool	operator==(const Vertex &other) const;
		};

		struct Builder{
			vector<Vertex>		vertices{};
			vector<uint32_t>	indices{};

			void	loadModel(const string &filepath, const int &color);
		};
	private:
		VeDevice				&_veDevice;
		unique_ptr<VeBuffer>	_vertexBuffer;
		uint32_t				_vertexCount;
		bool					_hasIndexBuffer = false;
		unique_ptr<VeBuffer>	_indexBuffer;
		uint32_t				_indexCount;
	
		VkImage					_textureImage;
		VkDeviceMemory			_textureImageMemory;
		VkImageView				_textureImageView;
		VkSampler				_textureSampler;

		void	copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void	transitionImageLayout(
			VkImage image,VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout
		);
		void	createImage(
			uint32_t width, uint32_t height,
			VkFormat format,
			VkImageTiling tiling,
			VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkImage &image,
			VkDeviceMemory &imageMemory
		);
		void	createTextureImages(const int &texture);
		void	createTextureImageView(void);
		void	createTextureSampler(void);
		void	createVertexBuffers(const vector<Vertex> &vertices);
		void	createIndexBuffers(const vector<uint32_t> &indices);
	public:
		static unique_ptr<VeModel>	createModelFromFile(
			VeDevice &device, const string &filepath, const int &color, const int &texture
		);

		VeModel(VeDevice &device, const VeModel::Builder &builder, const int &texture);
		~VeModel();

		void	bind(VkCommandBuffer commandBuffer);
		void	draw(VkCommandBuffer commandBuffer);
		VkDescriptorImageInfo	descriptorImageInfo(void);
};
