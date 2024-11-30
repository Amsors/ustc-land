#version 330

out vec4 glFragColor;

uniform sampler2D depthMap;
uniform sampler2D bgTex;
uniform mat4 shadowVp;
uniform mat4 iVp;
uniform vec2 windowSize;

float unpack(vec4 info) {
    const vec4 bitShift = vec4(1.0, 1.0 / 256.0, 1.0 / (256.0 * 256.0), 1.0 / (256.0 * 256.0 * 256.0));
    return dot(info, bitShift);
}

float shadow(vec3 pos) {
    float result = 0.0;
    result += pos.z - 0.001 > unpack(texture(depthMap, pos.xy)) ? 0.5 : 1.0;
    return result;
}

void main() {
    vec4 fragPos = shadowVp * iVp * vec4(gl_FragCoord.xy * 2.0 / windowSize - 1.0, gl_FragCoord.z * 2.0 - 1.0, 1.0);
    fragPos /= fragPos.w;
    glFragColor = texture(bgTex, (gl_FragCoord.st - 0.5) / windowSize) * shadow((fragPos.xyz + 1.0) / 2.0);
}