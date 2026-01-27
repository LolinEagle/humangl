### VeFrameInfo — Per-frame Uniforms & Render Context

`VeFrameInfo` groups data and small POD structs used every frame for rendering:
camera matrices, lighting uniforms, and the transient rendering context passed
to systems that record draw commands.

---

### Types Defined

- PointLightEntity
  - position : vem::vec4 — world-space position (w can be used for padding).
  - color    : vem::vec4 — RGB color + intensity or padding.

- GlobalUbo
  - projection    : vem::mat4 — camera projection matrix.
  - view          : vem::mat4 — camera view matrix.
  - inverseView   : vem::mat4 — inverse of the view matrix (useful for effects).
  - ambientLightColor : vem::vec4 — ambient light (RGBA, alpha used as intensity here).
  - pointLights   : PointLightEntity[MAX_LIGHTS] — array of point lights (MAX_LIGHTS is project-defined).
  - numLights     : int — number of active point lights.
  - textureOn     : int — toggle flag for texture usage in shaders.

- FrameInfo
  - frameIndex        : int — current frame index (modulo MAX_FRAMES).
  - frameTime         : float — elapsed time or delta time for the frame.
  - commandBuffer     : VkCommandBuffer — command buffer used to record this frame.
  - camera            : VeCamera& — reference to the active camera for this frame.
  - globalDescriptorSet : VkDescriptorSet — descriptor set containing GlobalUbo (and related resources).
  - gameObject        : VeGameObject::Map& — reference to collection of game objects to render.

---

### Purpose & Usage

- GlobalUbo is the CPU-side representation of the uniform buffer uploaded to shaders each frame: projection/view matrices, lighting, and toggles.
- FrameInfo is a transient context object passed to renderers/systems when recording draw calls:
  - It centralizes the command buffer, camera, descriptor set and object list for that frame.
  - Systems use frameIndex/frameTime for time-dependent animation and frame-local resource indexing.
- PointLightEntity and the pointLights array support multiple dynamic lights; numLights controls how many entries shaders should sample.

---

### Integration Notes

- Fill GlobalUbo each frame from the active VeCamera and scene lights, update the GPU uniform buffer, then bind globalDescriptorSet before drawing.
- Pass a FrameInfo instance into per-frame draw/submit functions so they have access to the command buffer, camera, and game objects without global state.
- Ensure MAX_LIGHTS matches shader-side declarations to avoid buffer/array mismatch.
