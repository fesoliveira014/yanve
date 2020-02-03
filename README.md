# Yet Another (visual? voxel?) Engine

This is the repo for my toy rendering (and future voxel) engine. It is implemented in modern C++ (C++ 17) and uses OpenGL 4.5. The rendering structure and code was heavily based upon the fantastic [magnum](https://github.com/mosra/magnum/) engine by mosra, mostly due to how well he managed to wrap most of the OpenGL APIs and his elegant solution to the shader attribute types problem I was facing in my previous engines.

# Dependencies

- C++ 17 compiler (currently only MSVC 19.0+)
- OpenGL 4.5 driver support
- [GLM](https://glm.g-truc.net/)
- [SDL2](https://www.libsdl.org/)
- [Dear ImGUI](https://github.com/ocornut/imgui)
- [stb](https://github.com/nothings/stb) (for image loading)

# MIT Licence

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.