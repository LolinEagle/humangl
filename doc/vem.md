### vem — Vulkan Entity Model Format

`vem` is a custom asset format used by the project to describe 3D models,
their hierarchy, and rendering-related metadata in a way that is tightly
integrated with the engine.

---

### Core Responsibilities

- **Model Description**
  - Defines mesh references, vertex data layout, and indices
  - Stores per-model transform data (translation, rotation, scale)

- **Hierarchical Structure**
  - Supports parent–child relationships between model parts
  - Enables articulated models (e.g., humanoids with limbs)
  - Allows transforms to propagate through the hierarchy

- **Material & Texture Metadata**
  - References textures used by each mesh
  - Stores flags such as texture enable/disable states
  - Decouples material configuration from render systems

- **Engine Integration**
  - Parsed at load time to create `VeModel` instances
  - Instantiated as `VeGameObject` entries in the scene
  - Designed to work with `SimpleRender` and animation systems

- **Animation Support**
  - Provides named parts for programmatic manipulation
  - Enables runtime animation via transform offsets and rotations
  - Works seamlessly with the `Controller` walking and jumping logic

---

### Summary

`vem` acts as the bridge between authored model data and runtime entities,
providing a lightweight, engine-specific format for hierarchical models
and their rendering configuration.
