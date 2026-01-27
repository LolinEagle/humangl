### VeRenderer — High-level Render Loop & Swapchain Management

`VeRenderer` coordinates Vulkan render-time resources and operations: command
buffer allocation, swap chain recreation, viewport/scissor setup, and exposing
render-pass / command-buffer handles used during frame rendering.

---

### Primary Responsibilities

- Manage command buffers:
  - createCommandBuffers() — allocates one VkCommandBuffer per frame.
  - freeCommandBuffers() — frees and clears allocated command buffers.
  - getCurrentCommandBuffer() — returns the command buffer for the current frame.

- Swap chain lifecycle:
  - recreateSwapChain() — waits for a valid window extent, waits for device idle,
    and creates a new VeSwapChain (optionally preserving the old one). Validates
    that image/depth formats remain compatible.

- Render-pass and viewport helpers:
  - beginSwapChainRenderPass / endSwapChainRenderPass (begin shown partially):
    - Sets dynamic viewport/scissor and begins/ends the render pass for the
      current swapchain framebuffer.
  - getSwapchainRenderPass() — returns the VkRenderPass owned by VeSwapChain.
  - getAspectRatio() — returns the swapchain's aspect ratio for camera/projection.

- Frame handling:
  - Internally uses MAX_FRAMES and a _currentFrameIndex to manage per-frame
    resources and double/triple buffering semantics.

---

### Collaborations / Dependencies

- VeDevice — device handle, command pool, and device-level helpers.
- VeSwapChain — encapsulates swapchain images, format, extent, and render pass.
- VeWindow — provides the surface extent and window event loop integration (used
  when waiting for a non-zero extent).
- Uses GLFW for event waiting when the window is minimized (extent == 0).

---

### Usage Notes

- Call createCommandBuffers() during renderer initialization and freeCommandBuffers()
  during cleanup or before recreating the swap chain.
- Call recreateSwapChain() when the window is resized or the swapchain becomes
  incompatible (e.g., VK_ERROR_OUT_OF_DATE_KHR).
- Use getCurrentCommandBuffer() and getSwapchainRenderPass() during frame
  recording to bind pipelines and issue draw calls.
- Ensure device is idle (vkDeviceWaitIdle) before destroying or recreating swapchain
  resources to avoid race conditions.

---

### Design Intent

- Centralize boilerplate for swapchain and command-buffer management so higher-level
  render loop and scene code remain focused on draw logic and resource binding.
