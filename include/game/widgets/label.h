#pragma once
#include "nanogui/label.h"

class ClickableLabel final: public nanogui::Label {
public:
    using MouseButtonCallback = std::function<bool(const nanogui::Vector2i &, int, bool, int)>;

    ClickableLabel(Widget *parent, const std::string &caption, const std::string &quit = "sans", const std::string &entered = "sans-bold", int fontSize = -1, const MouseButtonCallback &callback = nullptr);
    bool mouse_button_event(const nanogui::Vector2i &p, int button, bool down, int modifiers) override;
    bool mouse_enter_event(const nanogui::Vector2i &p, bool enter) override;

private:
    MouseButtonCallback mouseButtonCallback;
    const std::string quitFont;
    const std::string enteredFont;
};
