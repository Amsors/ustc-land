#pragma once
#include "nanogui/theme.h"

/**
 * @param name 着色器文件名（不含后缀名，顶点着色器和片段着色器必须同名）
 * @param vs 传递顶点着色器内容
 * @param fs 传递片段着色器内容
 * @return 是否加载成功
 */
bool loadShaders(const std::string& name, std::string& vs, std::string& fs);

/**
 * @return 用于Bar控件的基础主题
 */
nanogui::Theme *createBarTheme(NVGcontext *ctx);
