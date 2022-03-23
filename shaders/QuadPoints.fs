#version 460 core

in vec2 uv;

out vec4 FragColor;

void main() {
    float alpha = 1.0 - smoothstep(0.95, 1.0, length(uv));
    FragColor = vec4(alpha);
}