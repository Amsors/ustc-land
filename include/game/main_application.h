#pragma once

#include <memory>

#include "logic/basic.h"
#include "nanogui/screen.h"
#include "widgets/bar.h"
#include "game/logic/mainlogic.h"

class MainApplication final: public nanogui::Screen {
public:
    enum State {
        TITLE, PREPARING, PLAYING, PAUSED, QUITTING, OVER
    };

    enum MouseState {
        NONE, LEFT, RIGHT
    };

    MainApplication();
    ~MainApplication() override;
    bool keyboard_event(int key, int scancode, int action, int modifiers) override;
    bool mouse_button_event(const nanogui::Vector2i &p, int button, bool down, int modifiers) override;
    bool mouse_motion_event(const nanogui::Vector2i &p, const nanogui::Vector2i &rel, int button, int modifiers) override;
    void draw(NVGcontext *ctx) override;
    void draw_contents() override;

    void showCard();
    bool addCard();
    void checkCard(std::vector<std::shared_ptr<Card>>& stack);
    void giveReward();
    
    void check_all_cards();

    [[nodiscard]] nanogui::Vector2f screenToWorldZ0(const nanogui::Vector2f &p) const {
        nanogui::Vector4f worldPos = iVp * nanogui::Vector4f(p.x() * 2.f / m_size.x() - 1, 1 - p.y() * 2.f / m_size.y(), .9f, 1.f);
        worldPos /= worldPos[3];
        return {
            (camera[0] * std::abs(worldPos[2]) + worldPos[0] * std::abs(camera[2])) / (std::abs(worldPos[2]) + std::abs(camera[2])),
            (camera[1] * std::abs(worldPos[2]) + worldPos[1] * std::abs(camera[2])) / (std::abs(worldPos[2]) + std::abs(camera[2]))
        };
    }

    void startGame();
    void quitGame();

    [[nodiscard]] State gameState() const {
        return state;
    }

private:
    const double TICK = .05;
    const std::vector<std::pair<nanogui::Vector2f, int>> BG_TEXTURE_POSITIONS = {
        {{25.21f, 12.3f}, 0}, {{13.5f, -2.34f}, 1}, {{-3.68f, 1.56f}, 2}, {{6.11f, 1.89f}, 3}, {{-19.16f, -3.72f}, 4}, {{-5.44f, 6.72f}, 5}, {{-21.73f, 3.68f}, 6},
        {{-35.57f, 10.48f}, 7}, {{-30.14f, -3.95f}, 8}, {{-6.13f, -16.82f}, 9}, {{26.87f, -2.76f}, 10}, {{10.88f, -18.2f}, 11}, {{-33.28f, -18.47f}, 12}
    };

    std::vector<nanogui::ref<nanogui::Texture>> bgTextures;
    nanogui::Matrix4f iVp;
    nanogui::RenderPass *renderPass;
    nanogui::Shader *bgShader, *cardShader;
    nanogui::Vector3f camera;

    Bar *welcomeBar, *listBar, *infoBar; // HUD
    State state = TITLE;
    double lastFrame = 0.;
    MouseState mouseState = NONE;

    std::vector<std::vector<std::shared_ptr<Card>>> cards; // 越在下面的卡牌，对应的下标越小；不同牌堆之间的顺序不确定
    bool movingStack = false;
    std::queue<std::string> rewards;
    std::vector<std::vector<std::shared_ptr<Card>>*> waitingStacks;
};
