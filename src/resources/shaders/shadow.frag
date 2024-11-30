#version 330

in vec2 texCoord;
in vec4 cardCenter;
out vec4 glFragColor;

uniform mat4 iVp;
uniform vec2 windowSize;
uniform vec3 cardSize;

vec4 pack(float depth) {
    vec4 coded;
    for(int i = 0; i < 4; i++) {
        depth *= 256.0;
        coded[i] = floor(depth) / 256.0;
        depth = fract(depth);
    }
    return coded;
}

void main() {
    vec4 fragPos = iVp * vec4(gl_FragCoord.xy * 2.0 / windowSize - 1.0, gl_FragCoord.z * 2.0 - 1.0, 1.0);
    fragPos /= fragPos.w;
    vec2 d = abs(fragPos.xy - cardCenter.xy) - cardSize.xy / 2.0 + cardSize.z;
    if(d.x <= 0 || d.y <= 0 || length(d) <= cardSize.z) {
        // 牌的可见部分，需要生成阴影
        glFragColor = pack(gl_FragCoord.z);
    } else {
        discard;
    }
}
