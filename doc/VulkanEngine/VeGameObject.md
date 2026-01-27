### VeGameObject — Scene Object & Components

`VeGameObject` represents an entity in the scene with transform, optional model,
and optional point-light data. It provides creation helpers, unique IDs, and
transform math used by rendering and lighting systems.

---

### Responsibilities

- Hold per-object state:
  - TransformComponent (translation, offset, jumpOffset, scale, rotation)
  - Optional shared_ptr<VeModel> for mesh/visual representation
  - Optional unique_ptr<PointLightComponent> for point light properties
  - Color (vem::vec3) used for lit/unlit rendering or debug visualization
  - Unique uint ID

- Provide factory helpers:
  - static createGameObject() — returns a new VeGameObject with a unique ID
  - static makePointLight(intensity, radius, color) — convenience factory that
    creates an object configured as a point light (sets scale.x = radius and
    attaches a PointLightComponent with lightIntensity)

- Expose identity:
  - getId() -> const uint

---

### Transform API & Math

- TransformComponent::mat4()
  - Builds the model matrix combining Translate * Ry * Rx * Rz * Scale
  - Uses Tait–Bryan Y (yaw), X (pitch), Z (roll) ordering
  - Includes offset and jumpOffset in the translation row

- TransformComponent::normalMatrix()
  - Returns a 3x3 matrix for transforming normals
  - Accounts for non-uniform scale by applying inverse scale to the rotation basis

Notes:
- Rotation/scale math uses the same trigonometric basis as VeCamera for
  consistent orientation conventions across the engine.

---

### Usage Notes

- Create objects via createGameObject() or specialized factories like makePointLight().
- Attach a VeModel via the shared_ptr<VeModel> when the object has a mesh.
- If object is a light, use _pointLight to store intensity; render/lighting code
  can iterate objects and check for non-null point light pointers.
- TransformComponent.mat4() and normalMatrix() should be called before uploading
  model matrices and normal matrices to GPU uniforms.

---

### Design Intent

- Simple, copy-disabled value object with move semantics to avoid accidental ID duplication.
- Centralizes per-entity transform and lightweight component data (model + point light).
- Keeps math local to the component for easy reuse and consistency.
