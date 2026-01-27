### PointLight — Point Light Rendering System

`PointLight` is a dedicated render system responsible for updating and
drawing point lights in the scene using Vulkan pipelines and push constants.

---

### Core Responsibilities

- **Pipeline Management**
  - Creates a Vulkan graphics pipeline specialized for point light rendering
  - Defines a pipeline layout using:
    - A global descriptor set (camera, UBO data)
    - Push constants for per-light parameters

- **Light Data Handling**
  - Iterates over all game objects that contain point light components
  - Computes distance from the camera to each light source
  - Sorts lights by distance to ensure correct rendering order

- **Push Constants**
  - Sends per-light data to shaders using push constants:
    - Light position (world space)
    - Light color and intensity
    - Light radius (derived from object scale)
  - Updates push constants before each draw call

- **Rendering**
  - Binds the point light pipeline and global descriptor set
  - Renders each point light as a simple mesh (6 vertices)
  - Draws lights in reverse sorted order to handle blending correctly

- **Frame Integration**
  - Works within the main render loop via `FrameInfo`
  - Updates lighting data each frame before rendering

---

### Summary

`PointLight` encapsulates all logic required to manage and render dynamic
point lights, separating lighting concerns from mesh rendering and keeping
the rendering architecture modular.
