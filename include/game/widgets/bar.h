#pragma once

#include "nanogui/window.h"

class Bar: public nanogui::Window {
public:
    enum Position {
        HERE, MOVING, THERE
    };

    using SizeProvider = std::function<int(nanogui::Screen *)>;
    using DistanceProvider = std::function<double(int, int, int)>;

    Bar(nanogui::Widget *parent, double moveSpeed, const SizeProvider &width, const SizeProvider &height, const SizeProvider &margin, const DistanceProvider &distance0, const DistanceProvider &maxDistance);
    bool mouse_drag_event(const nanogui::Vector2i &p, const nanogui::Vector2i &rel, int button, int modifiers) override;
    void draw(NVGcontext *ctx) override;
    virtual void move(double deltaTime, bool isLeft) = 0;

    Position where() const {
        return distance == 0. ? HERE : distance == MAX_DISTANCE ? THERE : MOVING;
    }

protected:
    const double MOVE_SPEED;
    const int WIDTH, HEIGHT, MARGIN;
    const double MAX_DISTANCE;
    double x = 0., y = 0.;
    double distance;
};

class WelcomeBar final: public Bar {
public:
    WelcomeBar(nanogui::Widget *parent, double moveSpeed);
    void move(double deltaTime, bool isLeft) override;
};

class ListBar final: public Bar {
public:
    ListBar(nanogui::Widget *parent, double moveSpeed);
    void move(double deltaTime, bool isLeft) override;
    void updateInfo();

private:
    Widget *attribute, *advancement;
};
