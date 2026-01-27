### VeWindow — GLFW Window Wrapper & Surface Provider

`VeWindow` encapsulates GLFW window creation and handles window-related events and
viewport size state for the Vulkan renderer. It also creates the Vulkan surface
used by the swapchain.

---

### Core Responsibilities

- Initialize and destroy a GLFW window with a requested width and height.
- Track framebuffer resize events:
  - Uses a static GLFW callback to set an internal `_framebufferResized` flag.
  - Provides wasWindowResized() and resetWindowResizedFlag() for the renderer to query and clear the flag.
- Provide current drawable extent:
  - getExtent() returns a VkExtent2D used when creating or recreating the swapchain.
  - Handles the case where the window is minimized (extent may be zero) — renderer should wait for a non-zero extent.
- Window lifecycle:
  - shouldClose() to check GLFW window-close requests.
- Provide raw GLFW access:
  - getGLFWwindow() returns the underlying GLFWwindow* for input handling or integration.
- Create Vulkan surface:
  - createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) — wraps platform-specific surface creation using GLFW.

---

### Public API (summary)

- VeWindow(int width, int height)
- ~VeWindow()
- VkExtent2D getExtent()
- bool shouldClose()
- bool wasWindowResized()
- GLFWwindow* getGLFWwindow() const
- void resetWindowResizedFlag()
- void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)

---

### Usage Notes

- Call createWindowSurface() after Vulkan instance creation, before swapchain setup.
- When the renderer sees wasWindowResized() true (or swapchain returns VK_ERROR_OUT_OF_DATE_KHR),
  call resetWindowResizedFlag() and recreate the swapchain.
- On minimized windows (extent width/height == 0), the renderer should wait (e.g., via glfwWaitEvents()) until extent becomes non-zero before recreating the swapchain.
- Use getGLFWwindow() for input callbacks and integration with other systems.

---

### Design Intent

- Centralize window & surface boilerplate and hide GLFW-specific details from higher-level renderer code, while exposing simple, synchronous helpers for the swapchain lifecycle.
