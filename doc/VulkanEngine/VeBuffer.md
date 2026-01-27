### VeBuffer — Vulkan GPU Buffer Wrapper

`VeBuffer` is a lightweight RAII wrapper around a Vulkan buffer and its
device memory. It provides allocation, alignment, CPU mapping, and helper
operations used by the engine for uniform/storage/per-instance data.

---

### Core Responsibilities

- **Buffer allocation**
  - Creates a VkBuffer and allocates VkDeviceMemory via `VeDevice::createBuffer`.
  - Computes and applies per-instance alignment for dynamic or per-object buffers.

- **Alignment handling**
  - Uses a static helper `getAlignment(size, min)` to align instance size to a
    provided minimum offset alignment (used for dynamic UBOs or device limits).
  - `_alignmentSize` = aligned instance size; `_bufferSize` = `_alignmentSize * instanceCount`.

- **CPU mapping & writes**
  - `map(size, offset)` maps buffer memory and stores a pointer in `_mapped`.
  - `unmap()` unmaps only if currently mapped.
  - `writeToBuffer(data, size, offset)` writes either the whole buffer (VK_WHOLE_SIZE)
    or a region at `offset` using the mapped pointer.

- **Memory synchronization**
  - `flush(size, offset)` flushes mapped memory ranges to make CPU writes visible to the GPU.

- **Descriptor helpers**
  - `descriptorBufferInfo(size, offset)` returns a VkDescriptorBufferInfo ready for descriptor updates.

- **Lifecycle management**
  - Destructor unmaps (if needed) and frees Vulkan resources: vkDestroyBuffer and vkFreeMemory.

---

### Constructor Parameters (summary)

- `VeDevice &device` — device wrapper used for allocation.
- `VkDeviceSize instanceSize` — size of one element (e.g., per-object UBO).
- `uint instanceCount` — number of elements to allocate space for.
- `VkBufferUsageFlags usageFlags` — buffer usage (uniform/storage/vertex, etc.).
- `VkMemoryPropertyFlags memoryPropertyFlags` — memory properties (HOST_VISIBLE, HOST_COHERENT...).
- `VkDeviceSize minOffsetAlignment` — requested alignment for each instance (default 1).

---

### Notes & Usage

- The class exposes `getBuffer()` to retrieve the VkBuffer handle for binding.
- Macros `VE_SIZE` and `VE_OFFSET` simplify defaulting to VK_WHOLE_SIZE / 0 in calls.
- Intended for uniform buffers, storage buffers, or per-instance uploads where alignment and manual flush are required.
- Caller must ensure correct use of `map`/`flush`/`unmap` and respect alignment when computing per-instance offsets.

---

### Summary

`VeBuffer` centralizes Vulkan buffer creation, aligned sizing for multi-instance data, CPU mapping/writes, and safe cleanup, making buffer usage simpler and less error-prone across the engine.
