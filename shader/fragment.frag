#version 450 core

out vec4 fragColor;
in vec3 position;

void main() {
    float sum = position.x + position.y + position.z;
    vec3 p = position;
    p /= sum;
    fragColor = vec4(p, 1.0f);
}