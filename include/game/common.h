#pragma once
#include <sstream>
#include <string>

#include "nanogui/vector.h"
#include "spdlog/fmt/bundled/core.h"
#include "spdlog/fmt/bundled/format.h"

inline const nanogui::Color BAR_BACKGROUND(0xFF, 0xF9, 0xE3, 0xFF);
inline const nanogui::Color BAR_TEXT(0x00, 0x00, 0x00, 0xFF);
using ImageData = std::unique_ptr<uint8_t[], void(*)(void *)>;

class GameObject {
public:
    // TODO: 可能需要更改析构方法的声明
    virtual ~GameObject() = default;
    virtual void calc(double deltaTime) = 0;
};

template<typename Value, size_t size>
struct fmt::formatter<nanogui::Matrix<Value, size>>: fmt::formatter<std::string> {
    auto format(const nanogui::Matrix<Value, size> &m, format_context &ctx) {
        std::ostringstream oss;
        oss << '[';
        for(size_t i = 0; i < size; i++) {
            oss << '[';
            for(size_t j = 0; j < size; j++) {
                oss << m.m[j][i];
                if(j != size - 1) {
                    oss << ", ";
                }
            }
            oss << ']';
            if(i != size - 1) {
                oss << ", ";
            }
        }
        oss << ']';
        return formatter<std::string>::format(oss.str(), ctx);
    }
};

template<typename Value, size_t size>
struct fmt::formatter<nanogui::Array<Value, size>>: fmt::formatter<std::string> {
    auto format(const nanogui::Array<Value, size> &m, format_context &ctx) {
        std::ostringstream oss;
        oss << '[';
        for(size_t i = 0; i < size; i++) {
            oss << m[i];
            if(i != size - 1) {
                oss << ", ";
            }
        }
        oss << ']';
        return formatter<std::string>::format(oss.str(), ctx);
    }
};

/**
 * 加载着色器文件
 *
 * @param name 着色器文件名（不含后缀名，顶点着色器和片段着色器必须同名）
 * @param vs 传递顶点着色器内容
 * @param fs 传递片段着色器内容
 * @return 是否加载成功
 */
bool loadShaders(const std::string &name, std::string &vs, std::string &fs);

void initTexturePaths();

std::string &getTexturePath(const std::string &name);
