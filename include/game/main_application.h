#pragma once

#include "nanogui/screen.h"
#include "widgets/bar.h"

class MainApplication final: public nanogui::Screen {
public:
    MainApplication();
    bool keyboard_event(int key, int scancode, int action, int modifiers) override;
    void draw(NVGcontext *ctx) override;
    void draw_contents() override;

private:
    const int BACKGORUND_TEXTURES = 13;
    const std::vector<nanogui::Vector2f> TEXTURE_POSITIONS = {
        {25.21f, 12.3f}, {13.5f, -2.34f}, {-3.68f, 1.56f}, {6.11f, 1.89f}, {-19.16f, -3.72f}, {-5.44f, 6.72f}, {-21.73f, 3.68f}, {-35.57f, 10.48f}, {-30.14f, -3.95f}, {-6.13f, -16.82f}, {26.87f, -2.76f}, {10.88f, -18.2f}, {-33.28f, -18.47f}
    };

    Bar *welcomeBar{};

    std::vector<nanogui::ref<nanogui::Texture>> textures;
    nanogui::Shader *shader;
    nanogui::RenderPass *renderPass;
    int msyh, msyhbd, hyswhand;
};
