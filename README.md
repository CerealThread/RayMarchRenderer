# RayMarchRenderer
 
## Overview

This is a renderer written in C++ using OpenGL API. It uses ray marching to show some of it's capabilities.


## Requirements
- Visual Studio 2022
- OpenGL 4.3+


## Installation
1. git clone https://github.com/CerealThread/RayMarchRenderer.git
2. Open Visual Studio project file using Visual Studio
3. Build and run using Visual Studio as Debug x64

## Usage
There are three different scenes that can be rendered with this project. Change the command line parameters the adjust which is rendered. This can be done in the Project Properties. It is in the "Debugging" section and labeled "Command Arguments".
Choose a value of 0, 1, or 2 to see each respective scene.
The mouse rotates the view, and the escape key will terminate the program.


## Included Libraries
This project contains the following libraries
- FastNoiseLite
- freeglut
- glew
- glfw3
- glm

## Attributions

Some sections of code are modified from Packt, as well as some from Joey de Vries (https://twitter.com/JoeyDeVriez) from https://learnopengl.com/

Mathmatical functions for determining signed distance functions for the object primitives were based on [Inigo Quilez's](https://iquilezles.org/) functions, found [here](https://iquilezles.org/articles/distfunctions/).
