#version 330

in vec3 positions;
out vec4 cardCenter;

uniform mat4 model;
uniform mat4 vp;

void main() {
    cardCenter = model * vec4(0.5, 0.5, 0.0, 1.0);
    gl_Position = vp * model * vec4(positions.xyz, 1.0);
}
