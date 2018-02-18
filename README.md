# OpenGL_1
### Computer Graphics assignment in OpenGL

For this exercise we implemented the rendering of a predefined cube and pyramid and a sphere from an .obj file.

### Alterations and choices

* The object translations are a bit altered to make better use of the entire viewing screen, to:
    * cube: (2,-1.5,-6)
    * pyramid: (-2,-0.5,-6)
    * sphere: (0,4,-15)
* We have added and used several abstraction classes in the refactoring process of the application. For example, we have specific classes for polygons, combinations of polygons, shapes for OpenGL and static definitions of standard shapes.
* The colours are directly derived from the original unit positions, XYZ for RGB respectively.
* All the controls work, except Shading. Additionally, scrolling can be used to control scaling.
* The function unitize() has been implemented.