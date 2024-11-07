#pragma once
#include "game/common.h"

class Card: public GameObject {
public:
    using Position = nanogui::Vector3f;
    using Angle = nanogui::Vector3f;

    explicit Card(const nanogui::Vector3f &pos = {0.f, 0.f, 0.f});
    // TODO: 在析构方法中保存数据
    ~Card() override = default;
    void calc(double deltaTime) override;

    void move(const nanogui::Vector2f &delta) {
        pos.x() += delta[0];
        pos.y() += delta[1];
    }

    [[nodiscard]] bool contains(const nanogui::Vector2f &cursor) const {
        return pos[0] <= cursor[0] && cursor[0] <= pos[0] + W && pos[1] <= cursor[1] && cursor[1] <= pos[1] + H;
    }

    [[nodiscard]] const Position &getPosition() const {
        return pos;
    }

    [[nodiscard]] const Angle &getAngle() const {
        return angle;
    }

    static const float W, H;

private:
    Position pos;
    Angle angle;
};
