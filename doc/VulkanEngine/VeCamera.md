### VeCamera — View & Projection Manager

`VeCamera` encapsulates camera matrices and transforms used by the renderer.
It provides an easy interface to set perspective projection, compute a view
matrix from position + Euler rotation (YXZ order), and access the inverse view
and camera world position.

---

### Core Responsibilities

- Maintain camera matrices:
  - Projection matrix (_projectionMatrix)
  - View matrix (_viewMatrix)
  - Inverse view (world) matrix (_inverseViewMatrix)

- Provide setters for common camera configurations:
  - setPerspectiveProjection(fovy, aspect, near, far)
    - Builds a standard perspective projection matrix using field of view,
      aspect ratio and near/far planes.
  - setViewYXZ(position, rotation)
    - Computes the view matrix from camera position and Euler rotation in
      Y (yaw), X (pitch), Z (roll) order and also fills the inverse view
      (world transform) for convenience.

- Provide getters used by rendering and culling systems:
  - getProjection() -> const mat4&
  - getView() -> const mat4&
  - getInverseView() -> const mat4&
  - getPosition() -> vec3 (extracted from inverse view matrix)

---

### Implementation notes

- setPerspectiveProjection constructs the projection matrix using:
  - tanHalfFovy = tan(fovy / 2)
  - matrix elements arranged to map depth to [0,1] (right-handed convention used in code).
- setViewYXZ builds orthonormal basis vectors u, v, w from Euler rotation and
  places camera position into the inverse view matrix to allow easy world-space queries.
- getPosition reads the camera world position directly from the inverse view matrix row 3.

---

### Usage tips

- Call setPerspectiveProjection when the viewport size or projection parameters change.
- Update setViewYXZ each frame with the camera position and rotation before rendering.
- Use getProjection() and getView() to upload camera matrices to uniform buffers.
- getInverseView() is useful when shaders or systems need the camera world transform.

---

### Summary

`VeCamera` centralizes projection/view math and exposes matrix/state accessors so the renderer and game logic can consistently obtain camera transforms.
