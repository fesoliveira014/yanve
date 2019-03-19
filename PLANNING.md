# Planning

## Core Systems

- Window Subsystem
 - SDL based
 - Only deals with with window related operations:
    - create and manage rendering context
    - viewport resize
    - clear screen
    - swap buffers
 - attributes:
  - SDL window
  - SDL context
  - title

- Input subsystem
 - SDL based
 - manages all input, mapping each key, button and window state that can be altered by user input
 - singleton, only one input manager can exist during the lifetime of the program
 - no memory allocation required
 - deal with "quit" signal
 - it is initialized by the window upon its creation by setting the viewport state

- GUI Subsystem
 - Imgui based
