### VeSwapChain — Swapchain, Framebuffers & Presentation

`VeSwapChain` encapsulates creation and management of the Vulkan swapchain,
its images, depth buffers, framebuffers, render pass, and presentation/synchronization
objects. It centralizes all swapchain-related boilerplate so the renderer can
focus on recording and submitting draw work.

---

### Core Responsibilities

- Create and configure the swapchain:
  - choose surface format (prefers VK_FORMAT_B8G8R8A8_SRGB / SRGB_NONLINEAR).
  - choose present mode (prefers MAILBOX, falls back to FIFO).
  - choose extent from surface capabilities.

- Manage image resources:
  - Acquire swapchain images and create corresponding image views.
  - Create depth images, allocate memory and create depth image views for each
    framebuffer.

- Build render pass and framebuffers:
  - Create a render pass with a color attachment and a depth attachment.
  - Create framebuffers pairing each swapchain image view with its depth view.

- Synchronization & presentation:
  - Create per-frame semaphores and fences (_imageAvailableSemaphores,
    _renderFinishedSemaphores, _inFlightFences, _imagesInFlight).
  - Submit command buffers and present images via vkQueuePresentKHR.
  - Manage _currentFrame index for MAX_FRAMES in-flight frames.

- Swapchain lifecycle:
  - Support construction from an old swapchain to enable resource reuse
    when recreating the swapchain.
  - compareSwapFormate() to verify image & depth formats compatibility between
    old and new swapchains.

---

### Public API (summary)

- Constructors:
  - VeSwapChain(VeDevice &deviceRef, VkExtent2D extent)
  - VeSwapChain(VeDevice &deviceRef, VkExtent2D extent, shared_ptr<VeSwapChain> old)
- Destructor: cleans swapchain images, views, depth resources, framebuffers, sync objects.
- Presentation helpers (used by rendering loop):
  - present(image index, VkCommandBuffer) — submit & present (implementation shown).
- Accessors:
  - getRenderPass(), extentAspectRatio(), imageCount(), getFrameBuffer(idx), etc. (typical accessors implied).

---

### Collaborations / Dependencies

- VeDevice — device handle and helper for image creation/memory allocation.
- VeRenderer — calls into VeSwapChain to begin/end render pass and present frames.
- GLFW / platform surface — provides surface capabilities and extents.

---

### Usage Notes & Considerations

- Recreate the swapchain when the window is resized or VK_SUBOPTIMAL/VK_ERROR_OUT_OF_DATE_KHR occurs.
- Ensure vkDeviceWaitIdle() is used before destroying or replacing the swapchain to avoid races.
- Prefer mailbox present mode when available for lower latency; FIFO guarantees V-sync.
- compareSwapFormate() helps detect incompatible format changes that require broader resource re-creation.
