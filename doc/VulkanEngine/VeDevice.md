### VeDevice — Vulkan Device & Helper Wrapper

`VeDevice` is the central wrapper around Vulkan instance/physical/logical device state
used across the engine. It encapsulates device initialization, queue management,
resource creation helpers, and common Vulkan utilities so higher-level systems
(allocators, buffers, descriptors, command submission) can remain concise.

---

### Core Responsibilities

- Initialize and own Vulkan objects required for a logical device:
  - VkInstance / VkPhysicalDevice (selection & querying)
  - VkDevice (logical device) and device-level features
  - Queue handles for graphics, compute and present (as applicable)
  - VkSurfaceKHR handling (if the project uses a window surface)
- Provide accessors for underlying Vulkan handles:
  - device() -> VkDevice
  - physicalDevice() -> VkPhysicalDevice (or equivalent)
  - graphicsQueue(), presentQueue(), etc.
- Memory & resource helpers used by other classes:
  - createBuffer(size, usageFlags, memoryPropertyFlags, VkBuffer&, VkDeviceMemory&)
    - (used by VeBuffer)
  - findMemoryType(typeFilter, properties) -> uint32_t
- Command helpers for short-lived operations:
  - beginSingleTimeCommands() / endSingleTimeCommands() pattern
  - create/destroy command pools if needed
- Synchronization and cleanup:
  - Proper destruction of owned Vulkan objects in destructor
  - Optionally exposes convenience methods for fences/semaphores

---

### Typical Public API (conceptual)

- Constructors / destructor to initialize & tear down the Vulkan device.
- VkDevice device() const — retrieve logical device for raw Vulkan calls.
- VkPhysicalDevice physicalDevice() const — for queries needing physical device.
- void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &memory)
- uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
- VkQueue getGraphicsQueue(), getPresentQueue(), etc.
- Command helpers used for staging uploads and short-lived command buffers.

---

### Usage Notes

- Higher-level classes (VeBuffer, VeDescriptors, etc.) rely on `VeDevice` to:
  - Create buffers and allocate memory with appropriate memory types.
  - Provide the VkDevice handle when calling raw Vulkan functions.
  - Execute single-use command buffers for resource uploads.
- `VeDevice` centralizes error handling and capability checks (queue families, device features).
- It is expected to be long-lived and passed by reference into resource objects.

---

### Design Intent

- Reduce boilerplate: consolidate instance/device selection, queue discovery,
  memory-type selection, and common creation patterns.
- Provide safe ownership and deterministic cleanup of Vulkan resources.
- Expose small, well-documented helper methods so engine subsystems don't duplicate Vulkan logic.

---

### Summary

`VeDevice` is the project's Vulkan device abstraction and utility hub. It manages the logical device and queues, offers memory and resource creation helpers (like createBuffer used by VeBuffer), and provides convenient command/memory utilities so the renderer and resource classes can operate at a higher level of abstraction.
