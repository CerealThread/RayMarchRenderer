#version 430 core

layout (location = 0) in vec4 position;

uniform float time;
uniform vec2 Resolution;

out vec4 fColor;
out float uTime;
out vec2 iResolution;
void main(void)
{
    gl_Position = position;
    fColor = position;
    uTime = time;
}
