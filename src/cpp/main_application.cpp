#include "game/main_application.h"

#include <iostream>

#include "game/common.h"
#include "GLFW/glfw3.h"
#include "nanogui/button.h"
#include "nanogui/layout.h"
#include "nanogui/renderpass.h"
#include "nanogui/shader.h"
#include "nanovg/stb_image.h"
#include "spdlog/spdlog.h"

MainApplication::MainApplication():
    nanogui::Screen({300, 200}, "USTC Land", false, false), camera(0, -4, -40),
    msyh(nvgCreateFont(m_nvg_context, "msyh", R"(fonts\msyh.ttf)")), msyhbd(nvgCreateFont(m_nvg_context, "msyhbd", R"(fonts\msyhbd.ttf)")), hyswhand(nvgCreateFont(m_nvg_context, "hyswhand", R"(fonts\hyswhand.ttf)")) {
    SPDLOG_LOGGER_TRACE(spdlog::get("render"), "Main window created! size of the window: (x: {}, y: {}); pixel ratio: {:.2}", m_size.x(), m_size.y(), m_pixel_ratio);

    std::string bvs, bfs, cvs, cfs;
    loadShaders("background", bvs, bfs);
    loadShaders("card", cvs, cfs);
    renderPass = new nanogui::RenderPass({this});
    renderPass->set_clear_color(0, {0x85, 0xA8, 0x87, 0xFF});
    bgShader = new nanogui::Shader(renderPass, "background", bvs, bfs, nanogui::Shader::BlendMode::AlphaBlend);
    cardShader = new nanogui::Shader(renderPass, "card", cvs, cfs, nanogui::Shader::BlendMode::AlphaBlend);

    if(m_fullscreen) {
        m_size /= m_pixel_ratio;
    }
    welcomeBar = new WelcomeBar(this, 500.);
    listBar = new ListBar(this, 440.);
    infoBar = new InfoBar(this, 440.);
    perform_layout();

    for(int i = 1; i <= 13; i++) {
        nanogui::Vector2i size;
        int channels;
        const std::string &imageFile = fmt::format("textures/background/{}.png", i);
        const ImageData texture_data(stbi_load(imageFile.c_str(), &size.x(), &size.y(), &channels, 0), stbi_image_free);
        assert(channels == 4);
        auto *tex = new nanogui::Texture(nanogui::Texture::PixelFormat::RGBA, nanogui::Texture::ComponentFormat::UInt8, size, nanogui::Texture::InterpolationMode::Bilinear, nanogui::Texture::InterpolationMode::Bilinear);
        tex->upload(texture_data.get());
        bgTextures.emplace_back(tex);
    }

    constexpr uint32_t indices[] = {
        0, 1, 2, 0, 2, 3
    };
    constexpr float positions[] = {
        0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f
    };
    constexpr float texCoords[] = {
        1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f
    };

    const nanogui::Matrix4f p = nanogui::Matrix4f::perspective(.78539816f, 10.f, 60.f, m_size.x() * 1.f / m_size.y());
    const nanogui::Matrix4f vp =
        p * nanogui::Matrix4f::look_at(camera, nanogui::Vector3f(0, 0, 0), nanogui::Vector3f(0, 1, 0));
    iVp = nanogui::Matrix4f::inverse(vp);

    bgShader->set_buffer("indices", nanogui::VariableType::UInt32, {3 * 2}, indices);
    bgShader->set_buffer("positions", nanogui::VariableType::Float32, {4, 3}, positions);
    bgShader->set_buffer("texCoords", nanogui::VariableType::Float32, {4, 2}, texCoords);
    bgShader->set_uniform("vp", vp);

    cardShader->set_buffer("indices", nanogui::VariableType::UInt32, {3 * 2}, indices);
    cardShader->set_buffer("positions", nanogui::VariableType::Float32, {4, 3}, positions);
    // cardShader->set_buffer("texCoords", nanogui::VariableType::Float32, {4, 2}, texCoords);
    cardShader->set_uniform("vp", vp);
}

MainApplication::~MainApplication() {
    delete bgShader;
    delete cardShader;
    delete renderPass;
}

bool MainApplication::keyboard_event(const int key, const int scancode, const int action, const int modifiers) {
    if(Screen::keyboard_event(key, scancode, action, modifiers)) {
        return true;
    }
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if(state == PLAYING) {
            quitGame();
        } else {
            set_visible(false);
        }
        return true;
    }
    return false;
}

bool MainApplication::mouse_button_event(const nanogui::Vector2i &p, const int button, const bool down, const int modifiers) {
    if(Screen::mouse_button_event(p, button, down, modifiers)) {
        return true;
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
        if(down) {
            mouseState = RIGHT;
            switch(state) {
                case PLAYING:
                    cards.emplace_back(std::make_shared<Card>(nanogui::Vector3f(5.f, 7.f, 0.f)));
                    return true;
                default:
                    break;
            }
        } else {
            mouseState = NONE;
        }
    }
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
        if(down) {
            mouseState = LEFT;
        } else {
            mouseState = NONE;
        }
    }
    return false;
}

bool MainApplication::mouse_motion_event(const nanogui::Vector2i &p, const nanogui::Vector2i &rel, const int button, const int modifiers) {
    if(Screen::mouse_motion_event(p, rel, button, modifiers)) {
        return true;
    }
    switch(mouseState) {
        case LEFT: {
            const nanogui::Vector2f cursor = screenToWorldZ0(p - rel);
            for(auto it = cards.rbegin(); it != cards.rend(); ++it) {
                if((*it)->contains(cursor)) {
                    const nanogui::Vector2f posTo = screenToWorldZ0(p);
                    (*it)->move(posTo - cursor);
                    SPDLOG_LOGGER_WARN(spdlog::get("main"), "\ncursor: {}\npos: {}\nposTo: {}\ndelta: {}", cursor, (*it)->getPosition(), posTo, posTo - cursor);
                    break;
                }
            }
            break;
        }
        default:
            break;
    }
    return false;
}

void MainApplication::draw(NVGcontext *ctx) {
    Screen::draw(ctx);
}

void MainApplication::draw_contents() {
    bgShader->set_uniform("glfwTime", static_cast<float>(glfwGetTime()));
    renderPass->resize(framebuffer_size());
    renderPass->begin();

    // 绘制背景
    for(const auto &[pos, i]: BG_TEXTURE_POSITIONS) {
        nanogui::Matrix4f model =
            nanogui::Matrix4f::translate({pos.x(), pos.y(), 0}) *
            nanogui::Matrix4f::scale({bgTextures[i]->size().x() / 33.f, bgTextures[i]->size().y() / 33.f, 1.f});
        bgShader->set_uniform("model", model);
        bgShader->set_texture("tex", bgTextures[i].get());
        bgShader->begin();
        bgShader->draw_array(nanogui::Shader::PrimitiveType::Triangle, 0, 6, true);
        bgShader->end();
    }

    // 处理游戏内的时间变化
    const double now = glfwGetTime();
    const double deltaTime = now - lastFrame;
    lastFrame = now;

    switch(state) {
        case PREPARING:
            if(welcomeBar->where() == Bar::THERE) {
                listBar->move(deltaTime, false);
                infoBar->move(deltaTime, false);
                if(listBar->where() == Bar::HERE/* && infoBar->where() == Bar::HERE*/) {
                    state = PLAYING;
                }
            } else {
                welcomeBar->move(deltaTime, true);
            }
            break;
        case QUITTING:
            if(listBar->where() == Bar::THERE/* && infoBar->where() == Bar::THERE*/) {
                welcomeBar->move(deltaTime, false);
                if(welcomeBar->where() == Bar::HERE) {
                    state = TITLE;
                }
            } else {
                listBar->move(deltaTime, true);
                infoBar->move(deltaTime, true);
            }
            break;
        case PLAYING:
            for(const auto &card: cards) {
                card->calc(deltaTime);
            }
        // TODO: 绘制前景（卡牌、特效等）
            for(const auto &card: cards) {
                nanogui::Matrix4f model =
                    nanogui::Matrix4f::translate(card->getPosition()) *
                    nanogui::Matrix4f::scale({Card::W, Card::H, 1.f});
                cardShader->set_uniform("model", model);
                // cardShader->set_texture("tex", cardTextures[0].get());
                cardShader->begin();
                cardShader->draw_array(nanogui::Shader::PrimitiveType::Triangle, 0, 6, true);
                cardShader->end();
            }
            break;
        default:
            break;
    }

    renderPass->end();
}

void MainApplication::startGame() {
    state = PREPARING;
}

void MainApplication::quitGame() {
    state = QUITTING;
    // 清空卡牌列表，同时调用卡牌的析构函数，进行保存
    cards.clear();
}
