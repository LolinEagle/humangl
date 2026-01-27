### MainClass — Application Orchestrator

`MainClass` is the central class that initializes, configures, and runs the rendering
loop of the project. It wires together windowing, Vulkan rendering, scene loading,
camera control, input handling, and lighting.

---

### Core Responsibilities

- **Window & Device Setup**
  - Creates the GLFW window (`VeWindow`)
  - Initializes Vulkan device, renderer, and swapchain (`VeDevice`, `VeRenderer`)

- **Scene Management**
  - Loads one of two scenes at startup:
    - `HUMANGL`: articulated human model composed of multiple body parts
    - `SCOP`: simple scene with static objects
  - Uses `loadGameObjects()` to create `VeGameObject` instances from OBJ files
  - Maintains:
    - `_gameObjects`: all renderable objects
    - `_model`: pointers to specific body parts for animation/control

- **Rendering Systems**
  - Initializes:
    - `SimpleRender` for standard mesh rendering
    - `PointLight` for point light rendering and updates
  - Creates a global descriptor set layout and pool
  - Allocates per-frame uniform buffers (`GlobalUbo`)

- **Camera & Controller**
  - Creates a viewer `VeGameObject` used as the camera anchor
  - Uses `Controller` to:
    - Move the camera in the XZ plane
    - Handle mouse, keyboard, walking animation, jumping, and texture toggling
  - Updates camera view and perspective projection each frame

- **Main Loop (`run`)**
  - Polls input events
  - Computes frame delta time
  - Updates camera and controller state
  - Fills `GlobalUbo` with:
    - Projection, view, inverse view matrices
    - Texture enable flag
    - Point light data
  - Records and submits Vulkan draw commands
  - Handles frame synchronization and swapchain rendering

- **Shutdown**
  - Waits for the Vulkan device to become idle before exiting

---

### Summary

`MainClass` acts as the high-level coordinator of the engine:
it owns initialization, scene loading, per-frame updates, and rendering,
while delegating specialized tasks to controllers, render systems, and game objects.
