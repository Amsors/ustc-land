#version 330

in vec2 texCoord;
out vec4 glFragColor;

uniform sampler2D tex;
uniform vec4 bgColor;

void main() {
    vec4 t = texture(tex, texCoord);
    glFragColor = vec4(t.rgb * t.a + bgColor.rgb * (1.0 - t.a), 1.0);
}
