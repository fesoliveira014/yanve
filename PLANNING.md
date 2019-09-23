# Planning

## Core Systems

- Window Subsystem
   + SDL based
   + Only deals with with window related operations:
      * create and manage rendering context
      * viewport resize
      * ~clear screen~ (should be moved to framebuffer instead)
      * swap buffers
   + attributes:
      * SDL window
      * SDL context
      * title

- Input subsystem
   + SDL based
   + manages all input, mapping each key, button and window state that can be altered by user input
   + singleton, only one input manager can exist during the lifetime of the program
   + no memory allocation required
   + deal with "quit" signal
   + it is initialized by the window upon its creation by setting the viewport state

- GUI Subsystem
   + Imgui based
   
- GL Wrapping and Rendering subsystem
   + wraps OpengGL 4.5 objects
     * buffers
     * textures
     * framebuffers
     * mesh (VAO)
     * shaders
   + saves the state of the current rendering context
     * bound buffers
     * shader in use
     * existing vao
     * existing framebuffers
     * bound textures
     
- Primitive meshes
   + need to define a standard way of representing a mesh, as in a collection of buffers
   + planned primitives:
      * triangles
      * cubes
      * cones
      * spheres
      * euclidean polyhedra (8, 12, 20 faces)
   + csg operations (?)
   
- Resources subsystem
   + image object
      * wrapper around stl_image lib
   + model object
      * either use assimp or look for a better obj and wavefront file parser
   + shader file
      * shader files are resources like any other, should be loaded and manager by resource subsys
   + texture files
      * are texture simple images or something on their own?
