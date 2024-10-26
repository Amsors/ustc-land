#include <utility>

#include "game/widgets/label.h"

#include <iostream>

#include "spdlog/spdlog.h"

ClickableLabel::ClickableLabel(Widget *parent, const std::string &caption, const std::string &quit, const std::string &entered, const int fontSize, const MouseButtonCallback &callback):
    Label(parent, caption, quit, fontSize), mouseButtonCallback(callback), quitFont(quit), enteredFont(entered) {}

bool ClickableLabel::mouse_button_event(const nanogui::Vector2i &p, const int button, const bool down, const int modifiers) {
    if(mouseButtonCallback) {
        return mouseButtonCallback(p, button, down, modifiers);
    }
    return Label::mouse_button_event(p, button, down, modifiers);
}

bool ClickableLabel::mouse_enter_event(const nanogui::Vector2i &p, const bool enter) {
    if(enter) {
        SPDLOG_LOGGER_TRACE(spdlog::get("render"), "Mouse entered label. Changing font to {}", enteredFont);
        set_font(enteredFont);
    } else {
        SPDLOG_LOGGER_TRACE(spdlog::get("render"), "Mouse quit label. Changing font to {}", quitFont);
        set_font(quitFont);
    }
    return Label::mouse_enter_event(p, enter);
}
