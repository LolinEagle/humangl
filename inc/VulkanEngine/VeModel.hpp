#pragma once

#include <VeDevice.hpp>
#include <VeBuffer.hpp>

using namespace std;
using Bindings = vector<VkVertexInputBindingDescription>;
using Attributes = vector<VkVertexInputAttributeDescription>;
using uchar = unsigned char;

class VeModel{
public:
	struct Vertex{
		vem::vec3	position, color, normal;
		vem::vec2	uv;
		// Id of the texture
		uint		texId = 0;

		static Bindings		getBindingDescriptions(void);
		static Attributes	getAttributeDescriptions(void);

		bool	operator==(const Vertex &other) const;
	};

	struct Builder{
		vector<Vertex>	vertices{};
		vector<uint>	indices{};

		void	loadModel(const string &filepath, const int &color);
	};
private:
	VeDevice				&_veDevice;
	unique_ptr<VeBuffer>	_vertexBuffer;
	uint					_vertexCount;
	bool					_hasIndexBuffer = false;
	unique_ptr<VeBuffer>	_indexBuffer;
	uint					_indexCount;

	VkImage					_textureImage;
	VkDeviceMemory			_textureImageMemory;
	VkImageView				_textureImageView;
	VkSampler				_textureSampler;

	void	copyBufferToImage(VkBuffer buf, VkImage image, uint w, uint h);
	void	transitionImageLayout(
		VkImage image, VkFormat f,
		VkImageLayout oldLayout, VkImageLayout newLayout
	);
	void	createImage(
		uint w, uint h,
		VkFormat format,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkImage &image,
		VkDeviceMemory &imageMemory
	);
	void	createTextureImages();
	void	createTextureImageView(void);
	void	createTextureSampler(void);
	void	createVertexBuffers(const vector<Vertex> &vertices);
	void	createIndexBuffers(const vector<uint> &indices);
public:
	static unique_ptr<VeModel>	createModelFromFile(
		VeDevice &device, const string &file,
		const int &color, const int &texture
	);

	VeModel(VeDevice&, const VeModel::Builder&, const int&);
	~VeModel();

	void	bind(VkCommandBuffer cb);
	void	draw(VkCommandBuffer cb);
	VkDescriptorImageInfo	descriptorImageInfo(void);

	static unique_ptr<VeModel>	createCubeModel(VeDevice &device, const TexIdRaw texId);
};
