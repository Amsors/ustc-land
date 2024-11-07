#version 330

in vec2 texCoords;
in vec3 positions;
//out vec2 texCoord;

uniform mat4 model;
uniform mat4 vp;

void main() {
    //texCoord = texCoords;
    gl_Position = vp * model * vec4(positions.xyz, 1.0);
}
