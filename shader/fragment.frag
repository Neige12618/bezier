#version 450 core

out vec4 fragColor;
in vec3 position;

void main() {
    float sum = pow(position.x, 2) + pow(position.y, 2) + pow(position.z, 2);
    sum = sqrt(sum + 1);
//    fragColor = vec4(position.x * position.x / sum, position.y * position.y / sum, position.z * position.z / sum, 1.0f);
    fragColor = vec4(position, 1.0f);
}