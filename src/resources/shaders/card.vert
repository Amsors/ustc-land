#version 330

in vec2 texCoords;
in vec3 positions;
out vec4 cardCenter;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 vp;

void main() {
    texCoord = texCoords;
    cardCenter = model * vec4(0.5, 0.5, 0.0, 1.0);
    gl_Position = vp * model * vec4(positions.xyz, 1.0);
}
