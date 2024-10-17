#pragma once
#include "main_canvas.h"
#include "nanogui/screen.h"

class MainApplication final: public nanogui::Screen {
public:
    MainApplication();
    bool keyboard_event(int key, int scancode, int action, int modifiers) override;
    void draw(NVGcontext *ctx) override;

private:
    MainCanvas *canvas;
};
