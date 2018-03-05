# OpenGL_2
### Computer Graphics assignment in OpenGL

Changes to framework:

- Passed normal values to vert shader instead of arbitrary rgb values.
- Implemented the changing of shader programs inline with the GUI.
- Added ambient lighting component to output of vert shader for Gouraud.
- Used normal values to calculate specular and diffuse components in vert shader for Gouraud.
- Added ambient lighting component to output of frag shader for Phong.
- Used normal values to calculate specular and diffuse components in frag shader for Gouraud.
- Loaded diffuse texture for cat and wrapped it to cat mesh for Gouraud and Phong.