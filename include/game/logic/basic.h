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

    void moveTo(const nanogui::Vector2f &pos) {
        this->pos.x() = pos.x();
        this->pos.y() = pos.y();
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

    [[nodiscard]] const nanogui::Color &getColor() const {
        return color;
    };

    static const float W, H, D, R;

private:
    Position pos;
    Angle angle;

    // TODO: 需要删除，仅用于前期测试使用
    nanogui::Color color;
};
