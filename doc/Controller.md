### Controller — Player Input and Model Animation

`Controller` handles user input and translates it into camera movement,
model animation, and gameplay-like interactions such as walking and jumping.

---

### Core Responsibilities

- **Input Handling**
  - Reads keyboard input via GLFW for movement, rotation, walking, jumping, and toggles
  - Processes mouse scroll input through a GLFW callback for vertical movement
  - Hides the mouse cursor and manages input state transitions

- **Camera / Viewer Movement**
  - Rotates the viewer based on mouse input with configurable look speed
  - Clamps vertical rotation and wraps horizontal rotation using modulo
  - Moves the viewer in the XZ plane using forward, right, and up vectors
  - Normalizes movement direction to ensure consistent speed

- **Jumping & Falling Logic**
  - Implements a simple two-state system:
    - `_isJumping`: moves the model upward
    - `_isFalling`: brings the model back down
  - Applies vertical offsets (`jumpOffset`) to all model parts
  - Uses configurable jump speed and height limits

- **Walking Animation**
  - Animates the torso, head, and hat with forward motion
  - Applies sinusoidal swinging to arms and legs
  - Uses `swingModel()` to coordinate upper and lower limb motion
  - Resets all body part transforms when walking stops

- **Model Manipulation**
  - Operates on a map of body parts (`VeGameObject::MapPtr`)
  - Adjusts per-part:
    - Translation offsets
    - Rotation angles
    - Hierarchical limb relationships

- **State Toggles**
  - Toggles texture usage on key press with debouncing
  - Exposes texture state via `getTextureOn()`

---

### Summary

`Controller` acts as the bridge between user input and in-world behavior,
driving camera navigation, character movement, and procedural animation
for the articulated human model.
