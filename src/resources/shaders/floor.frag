#version 330

#define SAMPLES 25

out vec4 glFragColor;

uniform sampler2D depthMap;
uniform sampler2D bgTex;
uniform mat4 shadowVp;
uniform mat4 iVp;
uniform vec2 windowSize;

vec2 offsets[SAMPLES];
const int magic[] = int[SAMPLES](14, 18, 21, 24, 12, 19, 17, 22, 16, 2, 25, 23, 1, 3, 15, 7, 11, 20, 6, 8, 10, 5, 13, 9, 4);

float unpack(vec4 info) {
    const vec4 bitShift = vec4(1.0, 1.0 / 256.0, 1.0 / (256.0 * 256.0), 1.0 / (256.0 * 256.0 * 256.0));
    return dot(info, bitShift);
}

void poisson(vec2 seed) {
    float ANGLE_STEP = 2.0 * 3.14159265 / SAMPLES;
    float RADIUS_STEP = 1.0 / SAMPLES;
    float radius = RADIUS_STEP;
    for(int i = 0; i < SAMPLES; i++) {
        offsets[i] = vec2(cos(ANGLE_STEP * magic[i]), sin(ANGLE_STEP * magic[i])) * pow(radius, 0.75);
        radius += RADIUS_STEP;
    }
}

float shadow(vec3 pos) {
    float result = 0.0;
    poisson(pos.xy);
    for(int i = 0; i < SAMPLES; i++) {
        result += pos.z - 0.001 > unpack(texture(depthMap, pos.xy + offsets[i] * 9.0 / windowSize)) ? 0.4 : 1.0;
    }
    return result / SAMPLES;
}

void main() {
    vec4 fragPos = shadowVp * iVp * vec4(gl_FragCoord.xy * 2.0 / windowSize - 1.0, gl_FragCoord.z * 2.0 - 1.0, 1.0);
    fragPos /= fragPos.w;
    glFragColor = texture(bgTex, (gl_FragCoord.st - 0.5) / windowSize) * shadow((fragPos.xyz + 1.0) / 2.0);
}