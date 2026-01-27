### VeModel — Mesh, Buffers & Texture Helper

`VeModel` encapsulates a renderable 3D model: vertex/index buffers, texture
image + view + sampler, and helpers to bind/draw the model with Vulkan.

---

### Key Responsibilities

- Store mesh geometry (Vertex struct) and create GPU buffers:
  - Vertex contains position, color, normal, uv.
  - Static helpers: getBindingDescriptions(), getAttributeDescriptions().
  - createVertexBuffers() and createIndexBuffers() create device-local buffers
    using a host-visible staging buffer then copying to GPU memory.

- Load model geometry from files:
  - Builder::loadModel(filepath, color) parses an OBJ-like format into vertices
    and indices, builds normals if missing, and assigns vertex colors.

- Load and manage texture resources:
  - createTextureImages() loads PNG data, stages it, creates a VkImage and
    allocates memory, performs layout transitions and copies.
  - createTextureImageView() and createTextureSampler() prepare the image for
    shader sampling.
  - descriptorImageInfo() returns a VkDescriptorImageInfo for descriptor writes.

- Provide draw-time operations:
  - bind(VkCommandBuffer): bind vertex (and index) buffers.
  - draw(VkCommandBuffer): issue vkCmdDraw or vkCmdDrawIndexed depending on
    whether an index buffer exists.

- Lifetime and cleanup:
  - Constructor sets up texture, buffers and views from a Builder.
  - Destructor destroys sampler, image view, image and frees image memory.

---

### Public API (summary)

- static unique_ptr<VeModel> createModelFromFile(VeDevice &device, const string &file, int color, int texture)
- VeModel(VeDevice&, const VeModel::Builder&, const int &textureIndex)
- ~VeModel()
- void bind(VkCommandBuffer cb)
- void draw(VkCommandBuffer cb)
- VkDescriptorImageInfo descriptorImageInfo()

- VeModel::Vertex and VeModel::Builder nested types:
  - Vertex: position, color, normal, uv (+ equality operator).
  - Builder: vertices, indices, loadModel().

---

### Usage Notes & Considerations

- Depends on VeDevice for buffer creation, memory type selection, and single-time
  command helpers (used for copies and layout transitions).
- Uses staging buffers for transfers; requires VK_BUFFER_USAGE_TRANSFER_DST on
  final GPU buffers.
- descriptorImageInfo() is useful when writing descriptors for shaders that
  sample the model's texture.
- Throws runtime_error on Vulkan failures (image/buffer/sampler creation).
- Designed to be moved or held via unique_ptr for ownership clarity.
