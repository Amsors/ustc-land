#version 330

in vec2 texCoord;
in vec4 cardCenter;
out vec4 glFragColor;

uniform sampler2D tex;
uniform sampler2D depthMap;
uniform mat4 shadowVp;
uniform mat4 iVp;
uniform vec2 windowSize;
uniform vec3 cardSize;
uniform vec4 cardColor;

float unpack(vec4 info) {
    const vec4 bitShift = vec4(1.0, 1.0 / 256.0, 1.0 / (256.0 * 256.0), 1.0 / (256.0 * 256.0 * 256.0));
    return dot(info, bitShift);
}

// TODO: 使用PCF模拟软阴影
float shadow(vec3 pos) {
    float result = 0.0;
    result += pos.z - 0.001 > unpack(texture(depthMap, pos.xy)) ? 0.5 : 1.0;
    return result;
}

void main() {
    vec4 fragPos = iVp * vec4(gl_FragCoord.xy * 2.0 / windowSize - 1.0, gl_FragCoord.z * 2.0 - 1.0, 1.0);
    fragPos /= fragPos.w;
    vec2 d = abs(fragPos.xy - cardCenter.xy) - cardSize.xy / 2.0 + cardSize.z;
    if(d.x <= 0 || d.y <= 0 || length(d) <= cardSize.z) {
        vec4 img = texture(tex, texCoord);
        glFragColor = vec4(img.rgb * img.a + cardColor.rgb * (1 - img.a), 1.0);
    } else {
        discard;
    }
    fragPos = shadowVp * fragPos;
    fragPos /= fragPos.w;
    glFragColor.rgb *= shadow((fragPos.xyz + 1.0) / 2.0);
}
