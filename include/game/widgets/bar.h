#pragma once
#include "nanogui/window.h"
#include "nanovg/nanovg.h"

class Bar: public nanogui::Window {
public:
    explicit Bar(nanogui::Widget* parent);
    bool mouse_drag_event(const nanogui::Vector2i &p, const nanogui::Vector2i &rel, int button, int modifiers) override;
    void draw(NVGcontext *ctx) override;

private:
    NVGpaint stroke;
};

class WelcomeBar final: public Bar {
public:
    explicit WelcomeBar(nanogui::Widget* parent);
};
