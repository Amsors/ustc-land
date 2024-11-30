#version 330

in vec3 positions;

uniform mat4 vp;

void main() {
    gl_Position = vp * vec4(positions, 1.0);
}