#version 330

in vec3 position;
in vec3 color;
out vec4 frag_color;

uniform mat4 mvp;

void main() {
    frag_color = vec4(color, 1.0);
    gl_Position = mvp * vec4(position, 1.0);
}