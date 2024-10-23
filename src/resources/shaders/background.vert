#version 330

in vec2 texCoords;
in vec3 positions;
out vec2 texCoord;

uniform float glfwTime;
uniform mat4 model;
uniform mat4 vp;

void main() {
    texCoord = texCoords;
    if(texCoords.y > 0.5) {
        float phi0 = sin(dot(positions.xy, vec2(12.9898, 78.2336))) * 3.14159;
        float omega = cos(dot(positions.xy, vec2(13.5826, 23.5481))) * 0.5 + 1;
        gl_Position = vp * model * vec4(positions.x + sin(omega * (glfwTime) + phi0) * 0.05, positions.yz, 1.0);
    } else {
        gl_Position = vp * model * vec4(positions.xyz, 1.0);
    }
}
