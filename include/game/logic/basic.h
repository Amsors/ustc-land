#pragma once
#include "game/common.h"
#include "nanogui/object.h"

class Card final: public GameObject {
public:
    /*enum Type {
        ROLE, SPOT, ITEM
    };*/

    using Position = nanogui::Vector3f;
    using Angle = nanogui::Vector3f;

    explicit Card(std::string type, const std::string &name, const nanogui::Vector3f &pos = {0.f, 0.f, 0.f});
    // TODO: 不能在析构方法中保存数据
    ~Card() override = default;
    void calc(double deltaTime) override;

    void move(const nanogui::Vector3f &delta) {
        pos.x() += delta[0];
        pos.y() += delta[1];
        pos.z() += delta[2];
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
    }

    [[nodiscard]] const std::string &getName() const {
        return name;
    }

    [[nodiscard]] nanogui::ref<nanogui::Texture> getTexture() const {
        return texture;
    }

    static constexpr float W = 4.f; // 卡牌宽度
    static constexpr float H = 5.6; // 卡牌高度
    static constexpr float D = 1.2; // 卡堆中相邻卡牌的间距
    static constexpr float R = .6f; // 卡牌圆角半径
    static constexpr float TS = 3.f; // 卡牌材质大小

private:
    Position pos;
    Angle angle;
    const std::string name;
    nanogui::ref<nanogui::Texture> texture;
    //const Type type;
    std::string type;
    const nanogui::Color color;

    //static nanogui::Color colorSetter(const Type type) {
    //    switch(type) {
    //        case ROLE:
    //            return {0.9843f, 0.9608f, 0.8784f, 1.f}; // #FBF5E0
    //        case SPOT:
    //            return {0.9373f, 0.6902f, 0.4706f, 1.f}; // #EFB078
    //        case ITEM:
    //            return {0.9765f, 0.7412f, 0.6824f, 1.f}; // #F9BDAE
    //    }
    //    return {0.f, 0.f, 0.f, 0.f};
    //}
};
