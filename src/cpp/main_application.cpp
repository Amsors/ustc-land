#include "game/main_application.h"

#include "game/common.h"
#include "GLFW/glfw3.h"
#include "nanogui/button.h"
#include "nanogui/layout.h"
#include "nanogui/renderpass.h"
#include "nanogui/shader.h"
#include "nanovg/stb_image.h"
#include "spdlog/spdlog.h"

MainApplication::MainApplication():
    nanogui::Screen({0, 0}, "USTC Land", false, true),
    msyh(nvgCreateFont(m_nvg_context, "msyh", R"(fonts\msyh.ttf)")), msyhbd(nvgCreateFont(m_nvg_context, "msyhbd", R"(fonts\msyhbd.ttf)")), hyswhand(nvgCreateFont(m_nvg_context, "hyswhand", R"(fonts\hyswhand.ttf)")),
    welcomeBar(new WelcomeBar(this, 500.)), listBar(new ListBar(this, 440.)), infoBar(new InfoBar(this, 440.)) {
    SPDLOG_LOGGER_TRACE(spdlog::get("render"), "Main window created! size of the window: (x: {}, y: {}); pixel ratio: {:.2}", m_size.x(), m_size.y(), m_pixel_ratio);

    perform_layout();

    std::string vs, fs;
    loadShaders("background", vs, fs);
    renderPass = new nanogui::RenderPass({this});
    renderPass->set_clear_color(0, {0x85, 0xA8, 0x87, 0xFF});
    shader = new nanogui::Shader(renderPass, "background", vs, fs, nanogui::Shader::BlendMode::AlphaBlend);

    for(int i = 1; i <= BACKGORUND_TEXTURES; i++) {
        nanogui::Vector2i size;
        int channels;
        const std::string &imageFile = fmt::format("textures/background/{}.png", i);
        std::unique_ptr<uint8_t[], void(*)(void *)> texture_data(stbi_load(imageFile.c_str(), &size.x(), &size.y(), &channels, 0), stbi_image_free);
        assert(channels == 4);
        auto *tex = new nanogui::Texture(nanogui::Texture::PixelFormat::RGBA, nanogui::Texture::ComponentFormat::UInt8, size, nanogui::Texture::InterpolationMode::Bilinear, nanogui::Texture::InterpolationMode::Bilinear);
        tex->upload(texture_data.get());
        textures.emplace_back(tex);
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

    const nanogui::Matrix4f vp =
        nanogui::Matrix4f::perspective(45.f, .1f, 60.f, m_size.x() * 1.f / m_size.y()) *
        nanogui::Matrix4f::look_at(nanogui::Vector3f(0, -4, -40), nanogui::Vector3f(0, 0, 0), nanogui::Vector3f(0, 1, 0));

    shader->set_buffer("indices", nanogui::VariableType::UInt32, {3 * 2}, indices);
    shader->set_buffer("positions", nanogui::VariableType::Float32, {4, 3}, positions);
    shader->set_buffer("texCoords", nanogui::VariableType::Float32, {4, 2}, texCoords);
    shader->set_uniform("vp", vp);
}

bool MainApplication::keyboard_event(const int key, const int scancode, const int action, const int modifiers) {
    if(Screen::keyboard_event(key, scancode, action, modifiers)) {
        return true;
    }
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if(playing) {
            quitGame();
        } else {
            set_visible(false);
        }
        return true;
    }
    return false;
}

void MainApplication::draw(NVGcontext *ctx) {
    Screen::draw(ctx);
}

void MainApplication::draw_contents() {
    shader->set_uniform("glfwTime", static_cast<float>(glfwGetTime()));
    renderPass->resize(framebuffer_size());
    renderPass->begin();

    // 绘制背景
    for(int i = 0; i < BACKGORUND_TEXTURES; i++) {
        nanogui::Matrix4f model =
            nanogui::Matrix4f::translate({TEXTURE_POSITIONS[i].x(), TEXTURE_POSITIONS[i].y(), 0}) *
            nanogui::Matrix4f::scale({textures[i]->size().x() / 33.f, textures[i]->size().y() / 33.f, 1.f});
        shader->set_uniform("model", model);
        shader->set_texture("tex", textures[i].get());
        shader->begin();
        shader->draw_array(nanogui::Shader::PrimitiveType::Triangle, 0, 6, true);
        shader->end();
    }

    // 处理游戏内的时间变化
    const double now = glfwGetTime();
    const double deltaTime = now - lastFrame;
    lastFrame = now;
    if(playing) {
        timeProgress += deltaTime;
        if(timeProgress > TICK) {
            timeProgress -= TICK;
            ticks++;
            tick();
        }
    }

    if(playing) {
        if(welcomeBar->where() == Bar::THERE) {
            listBar->move(deltaTime, false);
            infoBar->move(deltaTime, false);
        } else {
            welcomeBar->move(deltaTime, true);
        }
    } else {
        if(listBar->where() == Bar::THERE/* && infoBar->where() == Bar::THERE*/) {
            welcomeBar->move(deltaTime, false);
        } else {
            listBar->move(deltaTime, true);
            infoBar->move(deltaTime, true);
        }
    }

    // TODO: 绘制前景（卡牌、特效等）

    renderPass->end();
}

void MainApplication::startGame() {
    ticks = 0;
    playing = true;
}

void MainApplication::quitGame() {
    playing = false;
    // TODO: 保存游戏进度
}

void MainApplication::tick() {}
