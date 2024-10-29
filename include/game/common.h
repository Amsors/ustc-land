#pragma once
#include <string>

#include "nanogui/vector.h"

inline const nanogui::Color BAR_BACKGROUND(0xFF, 0xF9, 0xE3, 0xFF);
inline const nanogui::Color BAR_TEXT(0x00, 0x00, 0x00, 0xFF);

class Tickable {
public:
    // TODO: 可能需要更改析构方法的声明
    virtual ~Tickable() = default;
    virtual void tick() = 0;
};

/**
 * @param name 着色器文件名（不含后缀名，顶点着色器和片段着色器必须同名）
 * @param vs 传递顶点着色器内容
 * @param fs 传递片段着色器内容
 * @return 是否加载成功
 */
bool loadShaders(const std::string &name, std::string &vs, std::string &fs);
