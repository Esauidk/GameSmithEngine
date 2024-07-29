![GitHub Workflow Status (with event)](https://img.shields.io/github/actions/workflow/status/Esauidk/GameSmithEngine/CI.yml)

# GameSmithEngine
A C++ Game Engine


## Installation
The source files for this engine is configured using premake. The premake file is included in this repo under the `third-party/bin` directory.
Each project has a `premake5.lua` file that contains the configuration definitions for the specific project.

Use `BuildProject.bat` to run premake and setup of the projects.

Platform Support:
- [X] Windows (Development done in Visual Studio)
- [ ] Mac
- [ ] Linux

## Game Smith Engine
This is the core project containing all of the code that runs the engine. Users will use code from this project to create custom code for their game.

Documentation can be found ~~here~~ (**TO BE MADE**)

Systems the engine currently supports:
- Rendering System
    - [X] DirectX12
    - [ ] Vulkan
    - [ ] OpenGL
- Material System
- Resource/Asset Management System
    - [X] Asset ID System
    - [X] Asset Serialization Support
    - [X] Loaded Asset Caching
    - [X] Asset Memory Management
    - [ ] User Memory Management
- Entity/Gameobject-Component Architecture
    - [X] Component Priority Updating
    - [X] GameObject & Component Serialization Support
    - [X] GameObject Management & Tracking
- Camera Support
    - [X] Orthographics
    - [X] Perspective
- Light Support 
    - [X] Directional 
    - [X] Point
    - [ ] Spot
    - [ ] Area
    - [ ] Multi-Lighting Support
- Console Logging
- Game Engine Events System
    - [X] Event Multi-Subscribers
    - [X] Event Dispatching
- Keyboard & Mouse Inputs
    - [X] Events
    - [X] Polling
- Frame Rate Limiting
- Custom User Content Libraries (Custom code)
    - [X] DLL Support
    - [ ] Static Library Support
- ImGUI Support
    - [X] Layers System

Some **components** built into the engine currently:
- Mesh Renderer: For rendering 3D Meshes (OBJ Support only)
- Camera: Attaches a camera to a gameobject

## Game Smith Editor
This is a simple application that links with the Game Engine DLL to enable creation of assets & content for a game compatible with the game smith engine. This is currently in a sandbox like state currently and can be used to visually test out user code

## Game Smith Test Suite
This is a google test suite application that runs a suite of unit tests made to test the correctness of code in the Game Smith Engine

## Test Zone
This is another sandbox application that links with the game smith engine. This is not a controled enviornment like the editor and can be used to run any code that interacts with the engine.
