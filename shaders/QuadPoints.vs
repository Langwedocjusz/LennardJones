#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

layout (location = 2) in vec4 aInstPos;

out vec2 uv;

uniform float scale_x;
uniform float scale_y;
uniform float scale_mult;

void main() {
    vec3 pos = scale_mult*vec3(scale_x*aPos.x, scale_y*aPos.y, 0.0);
    gl_Position = vec4(pos, 1.0) + aInstPos;
    uv = aUV;
}
