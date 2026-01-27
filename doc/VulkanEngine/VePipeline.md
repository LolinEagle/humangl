### VePipeline — Graphics Pipeline Builder & Binder

`VePipeline` creates and manages a Vulkan graphics pipeline from SPIR-V shader
files and a configuration struct. It wraps shader module creation, pipeline
assembly, and provides convenience defaults for common pipeline settings.

---

### Core Responsibilities

- Read SPIR-V files and create VkShaderModule for vertex & fragment shaders.
- Assemble VkGraphicsPipelineCreateInfo using:
  - Vertex input (bindings + attributes)
  - Input assembly, viewport, rasterization, multisampling
  - Depth/stencil and color blend state
  - Dynamic state (viewport, scissor)
  - Pipeline layout and render pass/subpass
- Create and destroy the VkPipeline and associated shader modules.
- Provide runtime binding of the pipeline to a command buffer.

---

### PipelineConfigInfo

- Central plain struct used to configure pipeline creation:
  - bindingDescription / attributeDescription (from VeModel::Vertex)
  - viewportInfo, inputAssemblyInfo, rasterizationInfo, multisampleInfo
  - colorBlendAttachment, colorBlendInfo, depthStencilInfo
  - dynamicStateEnables + dynamicStateInfo
  - pipelineLayout, renderPass, subpass index

- defaultPipelineConfigInfo(PipelineConfigInfo &config)
  - Fills sensible defaults for a typical forward-rendering pipeline.
  - Sets vertex input layouts using VeModel::Vertex helpers.

- enableAlphaBlending(PipelineConfigInfo &config)
  - Toggles colorBlendAttachment to enable standard SRC_ALPHA blending.

---

### Public API (summary)

- VePipeline(VeDevice &device, const string &vertPath, const string &fragPath, const PipelineConfigInfo &config)
- ~VePipeline()
- void bind(VkCommandBuffer commandBuffer)
- static void defaultPipelineConfigInfo(PipelineConfigInfo &config)
- static void enableAlphaBlending(PipelineConfigInfo &config)

---

### Usage Notes

- Provide a configured PipelineConfigInfo (or use defaultPipelineConfigInfo)
  and SPIR-V shader paths to construct a pipeline.
- Relies on VeDevice for Vulkan device handle and capabilities.
- Call bind() during command buffer recording before issuing draw calls.
- Keep pipeline creation relatively static — recreate only when render pass,
  shaders, or input layout change, as pipeline creation is expensive.

---

### Design Intent

- Encapsulate Vulkan pipeline boilerplate so renderer code remains concise.
- Expose small, composable configuration API enabling reuse across materials
  and shader variants.
