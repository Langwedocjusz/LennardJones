#version 460 core

in vec2 uv;

out vec4 FragColor;

uniform float smoothness;
uniform float scale_mult;

void main() {
    float alpha = 1.0 - smoothstep(1.0 - smoothness/scale_mult, 1.0, length(uv));
    FragColor = vec4(alpha);
}