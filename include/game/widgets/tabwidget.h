#pragma once
#include "nanogui/tabwidget.h"

class StyledTabWidget final: public nanogui::TabWidget {
public:
    explicit StyledTabWidget(nanogui::Widget *parent, const std::string &quitTab = "sans", std::string enteredTab = "sans-bold");
    void draw(NVGcontext *ctx) override;
    bool mouse_motion_event(const nanogui::Vector2i &p, const nanogui::Vector2i &rel, int button, int modifiers) override;
    void perform_layout(NVGcontext *ctx) override;

private:
    static const nanogui::Color UNFOCUSED;

    const std::string enteredTabFont;
    int hovering;
    int tabWidth;
};
