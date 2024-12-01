#version 330

#define SAMPLES 16

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

vec2 offsets[SAMPLES];

float unpack(vec4 info) {
    const vec4 bitShift = vec4(1.0, 1.0 / 256.0, 1.0 / (256.0 * 256.0), 1.0 / (256.0 * 256.0 * 256.0));
    return dot(info, bitShift);
}

void poisson(vec2 seed) {
    float ANGLE_STEP = 6.2831853 / SAMPLES;
    float RADIUS_STEP = 1.0 / SAMPLES;
    float angle = 6.2831853 * fract(dot(seed.xy, vec2(12.9898, 78.233)) * 43758.545312);
    float radius = RADIUS_STEP;
    for(int i = 0; i < SAMPLES; i++) {
        offsets[i] = vec2(cos(angle + ANGLE_STEP * i), sin(angle + ANGLE_STEP * i)) * pow(radius, 0.75);
        radius += RADIUS_STEP;
    }
}

float shadow(vec3 pos) {
    float result = 0.0;
    poisson(pos.xy);
    for(int i = 0; i < SAMPLES; i++) {
        result += pos.z - 0.001 > unpack(texture(depthMap, pos.xy + offsets[i] * 6.3 / windowSize)) ? 0.65 : 1.0;
    }
    return result / SAMPLES;
}

void main() {
    vec4 fragPos = iVp * vec4(gl_FragCoord.xy * 2.0 / windowSize - 1.0, gl_FragCoord.z * 2.0 - 1.0, 1.0);
    fragPos /= fragPos.w;
    vec2 d = abs(fragPos.xy - cardCenter.xy) - cardSize.xy / 2.0 + cardSize.z;
    if(d.x <= 0 || d.y <= 0 || length(d) <= cardSize.z) {
        vec4 img = texture(tex, texCoord);
        glFragColor = vec4(img.rgb * img.a + cardColor.rgb * (1 - img.a), 1.0);
        // 绘制纯黑色边框
        if((d.x > 0 || d.y > 0) && length(clamp(d, 0.0, cardSize.z)) > cardSize.z * 0.75) {
            glFragColor = vec4(0.0, 0.0, 0.0, 1.0);
        }
    } else {
        discard;
    }
    fragPos = shadowVp * fragPos;
    fragPos /= fragPos.w;
    glFragColor.rgb *= shadow((fragPos.xyz + 1.0) / 2.0);
}
