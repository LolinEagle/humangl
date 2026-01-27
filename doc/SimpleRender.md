### SimpleRender — Basic Mesh Rendering System

`SimpleRender` is the primary render system used to draw standard
game objects with meshes and materials in the scene.

---

### Core Responsibilities

- **Pipeline Creation**
  - Builds a Vulkan graphics pipeline using a vertex and fragment shader
  - Configures default pipeline states (input assembly, rasterization, depth)
  - Associates the pipeline with the active render pass

- **Pipeline Layout**
  - Creates a pipeline layout that includes:
    - One global descriptor set layout (camera and global UBO data)
    - A push constant range for per-object transform data
  - Uses `PushConstantData` to pass:
    - Model matrix
    - Normal matrix

- **Object Rendering**
  - Iterates over all `VeGameObject` instances in the scene
  - Skips objects without an attached model
  - Computes per-object transformation matrices
  - Pushes transform data to the GPU before each draw call

- **Descriptor Binding**
  - Binds the global descriptor set for each frame
  - Ensures shaders have access to camera matrices and global settings

- **Draw Submission**
  - Binds the pipeline and vertex/index buffers
  - Issues Vulkan draw commands for each renderable object

---

### Summary

`SimpleRender` provides a clean and minimal rendering path for standard
meshes, handling per-object transforms while relying on shared global
data for camera and scene-wide configuration.
