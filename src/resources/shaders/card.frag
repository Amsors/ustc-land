#version 330

//in vec2 texCoord;
out vec4 color;

// TODO: 需要删除，仅用于前期测试使用
uniform vec4 cardColor;
//uniform sampler2D tex;

void main() {
    color = cardColor;
}
