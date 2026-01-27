### VeDescriptors — Descriptor Layout, Pool & Writer Utilities

`VeDescriptors` groups helper classes that simplify Vulkan descriptor set layout
creation, descriptor pool management, and descriptor set writing.

---

### Components

- VeDescriptorSetLayout
  - Encapsulates a VkDescriptorSetLayout and its bindings.
  - Builder API:
    - addBinding(binding, type, stageFlags, count = 1)
    - build() -> unique_ptr<VeDescriptorSetLayout>
  - Stores bindings in an unordered_map for lookup by binding index.
  - Creates/destroys the VkDescriptorSetLayout in constructor/destructor.

- VeDescriptorPool
  - Wraps a VkDescriptorPool and exposes allocation/free/reset operations.
  - Builder API:
    - addPoolSize(descriptorType, count)
    - setPoolFlags(flags)
    - setMaxSets(count)
    - build() -> unique_ptr<VeDescriptorPool>
  - create/destruct pool in ctor/dtor and throws on creation failure.
  - Methods:
    - allocateDescriptor(descriptorSetLayout, VkDescriptorSet&): allocates one set.
    - freeDescriptors(vector<VkDescriptorSet>&): frees sets back to pool.
    - resetPool(): resets the pool for reuse.

- VeDescriptorWriter
  - Helper to collect writes (buffers/images) and update a VkDescriptorSet.
  - Constructed with a VeDescriptorSetLayout & VeDescriptorPool.
  - Methods:
    - writeBuffer(bind, VkDescriptorBufferInfo*): queues a buffer write.
    - writeImage(bind, VkDescriptorImageInfo*): queues an image write.
    - build(VkDescriptorSet&): allocates a set from the pool and applies writes.
    - overwrite(VkDescriptorSet&): applies queued writes to an existing set.
  - Uses the set layout's binding metadata to set descriptor types for writes.

---

### Usage Overview

1. Build a VeDescriptorSetLayout with required bindings.
2. Build a VeDescriptorPool with appropriate pool sizes and max sets.
3. Use VeDescriptorWriter to attach buffers/images to bindings and either:
   - build() to allocate & write a new descriptor set, or
   - overwrite() to update an existing set.

---

### Notes & Considerations

- The code uses a simple allocation approach; pool exhaustion is not auto-handled
  (comment suggests a PoolManager could recreate pools when full).
- Binding metadata is relied upon when composing VkWriteDescriptorSet entries.
- Error handling: constructors throw on Vulkan creation failures; allocateDescriptor
  returns false when allocation fails.
- Designed to reduce boilerplate when creating descriptor layouts, pools, and updates.

---

### Summary

`VeDescriptors` centralizes descriptor-related boilerplate into builder-style
APIs and a small writer utility, making descriptor layout creation, pool
management, and descriptor updates easier and less error-prone across the engine.
