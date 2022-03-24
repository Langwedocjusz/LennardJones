#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

layout (location = 2) in vec4 aInstPos;

out vec2 uv;

uniform float scale_x;
uniform float scale_y;
uniform float scale_mult;

void main() {
    gl_Position = vec4(scale_mult*aPos, 0.0, 1.0) + aInstPos;
    gl_Position.x *= scale_x;
    gl_Position.y *= scale_y;
    uv = aUV;
}
