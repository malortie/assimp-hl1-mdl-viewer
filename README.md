# Assimp Half-Life 1 model viewer

* [Credits](#Credits)
* [Prerequistes](#Prerequistes)
* [Dependencies](#Dependencies)
* [Building using CMake](#Building-using-CMake)
  * [CMake options](#CMake-options)
* [Custom user interface](#Custom-user-interface)

# Credits

Thanks to the following projects for the use of their libraries/frameworks.

* [Assimp](http://www.assimp.org/)
* [glm](https://glm.g-truc.net/)
* [NanoGUI](https://nanogui.readthedocs.io/en/latest/#)
* [glad](https://github.com/Dav1dde/glad)

# Prerequistes

* C++17 or higher
* OpenGL 3.3 or higher
* CMake 3.0 or higher (If using CMake)
* (Optional) Steam version of Half-Life 1. Required to load game models and sounds.

# Dependencies

This project uses the following dependencies:

* [Assimp](http://www.assimp.org/)
* [glm](https://github.com/g-truc/glm)
* [NanoGUI](https://github.com/wjakob/nanogui)

# Building using CMake

The following options can be added to CMake.

|Name|Description|Type|Default value|
|:-|:-|:-|:-|
|HLMDLVIEWER_GAME_EXECUTABLE_DIR|Specifies the game's executable directory. This is the directory that contains the game executable. This is used to locate custom resources such as sounds.|String|Empty|
|HLMDLVIEWER_USE_NANOGUI| By default, the project uses NanoGUI for the user interface. You may choose to disable this option, but you will need to implement your own user interface.|Boolean|ON|

# Custom user interface

Using the default user interface built on NanoGUI is recommended. However, if you wish to implement your own user interface for viewing HL1 models, you may implement the interface **HL1MDLViewerView**.

Example

```
class MyView : public HL1MDLViewerView 
{
public:
    // Implement interface...
};

int main()
{
    MyView view;
    HL1MDLViewerPresenter presenter(&view);
    MDLViewerApplication app(&presenter);
    return app.run();
}
```
