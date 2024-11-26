#version 330

in vec2 texCoord;
in vec4 cardCenter;
out vec4 glFragColor;

uniform sampler2D tex;
uniform mat4 iVp;
uniform vec2 windowSize;
uniform vec3 cardSize;
uniform vec4 cardColor;

void main() {
    vec4 fragPos = iVp * vec4(gl_FragCoord.xy * 2.0 / windowSize.xy - 1.0, gl_FragCoord.z * 2.0 - 1.0, 1.0);
    fragPos /= fragPos.w;
    vec2 d = abs(fragPos.xy - cardCenter.xy) - cardSize.xy / 2.0 + cardSize.z;
    if(d.x <= 0 || d.y <= 0 || length(d) <= cardSize.z) {
        vec4 img = texture(tex, texCoord);
        glFragColor = vec4(img.rgb * img.a + cardColor.rgb * (1 - img.a), 1.0);
    }
}
