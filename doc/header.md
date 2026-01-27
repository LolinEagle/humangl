### header & main — Application Entry Point

The `header` and `main` files define the entry point of the application and
connect the engine framework to the runtime execution flow.

---

### header

- **Declarations**
  - Declares the main application class and its public interface
  - Exposes configuration constants used at startup
  - Includes required engine headers to reduce coupling in `main`

- **Separation of Concerns**
  - Keeps class definitions and interfaces separate from implementation
  - Ensures clean compilation boundaries across the project

---

### main

- **Program Entry**
  - Contains the `main()` function
  - Serves as the first executed code when the application starts

- **Application Lifecycle**
  - Instantiates the main application class
  - Invokes the initialization and run loop
  - Ensures proper shutdown when execution ends

- **Error Handling**
  - Wraps execution in a `try / catch` block
  - Reports fatal errors to standard output
  - Returns appropriate exit codes on failure

---

### Summary

Together, `header` and `main` define how the application starts,
clearly separating declaration, initialization, execution, and shutdown
responsibilities.
