# Laboratory work 3

This work has one GUI program based on wxWidgets.

Program can operate with multiple shapes (triangles and rectangles). It can:

1. Create multiple new shapes
2. Clone it
3. Delete it
4. Toggle visibility of shape
5. Change color for current shape
6. Return default color
7. Toggle trace of shape
8. Change color when intersecting
9. Deformate shape on edge of border area

In future program will support aggregation, saving/restoring current state of shapes to/from config and deformation when intersecting.

Source files can separated on four categories:

1. Compilation rules:
    1. CMakeLists.txt
    
2. Interface implementation:
    1. MyApp.{h,cpp}
    2. MyFrame.{h,cpp} - Main window
    
3. Logic implementation:
    1. Shapes.{h,cpp} - Class Shape, trace creator and container for Shapes
    2. ShapeChilds.{h,cpp} - child classes of class Shape.
    
4. Additional headers:
    1. libgaura.h - Array class and Pointer class, that works same as vector and auto_ptr/unique_ptr
    2. wx_pch.h - Can be precompiled for future fast compilation

This is a screenshot of program:

![Screenshot](Screenshot.png)