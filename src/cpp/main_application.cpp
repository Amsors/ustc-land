#include "main_application.h"

#include <iostream>

#include "GLFW/glfw3.h"
#include "nanogui/button.h"
#include "nanogui/layout.h"
#include "nanogui/window.h"

MainApplication::MainApplication():
    nanogui::Screen({800, 600}, "NanoGUI Test", false) {
    auto *window = new nanogui::Window(this, "Main Window");
    window->set_position({0, 0});
    window->set_layout(new nanogui::GroupLayout());

    canvas = new MainCanvas(window);
    canvas->set_background_color({100, 100, 100, 255});
    canvas->set_fixed_size({400, 400});

    auto *tools = new Widget(window);
    tools->set_layout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal, nanogui::Alignment::Middle, 0, 5));

    auto *b0 = new nanogui::Button(tools, "Random Background");
    b0->set_callback([this]() {
        canvas->set_background_color({rand() % 256, rand() % 256, rand() % 256, 255});
    });
    auto *b1 = new nanogui::Button(tools, "Random Rotation");
    b1->set_callback([this]() {
        canvas->setRotation(3.14159265358979323846 * rand() / static_cast<float>(RAND_MAX));
    });

    perform_layout();
}

bool MainApplication::keyboard_event(const int key, const int scancode, const int action, const int modifiers) {
    if(Screen::keyboard_event(key, scancode, action, modifiers)) {
        return true;
    }
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        set_visible(false);
        return true;
    }
    return false;
}

void MainApplication::draw(NVGcontext *ctx) {
    Screen::draw(ctx);
}
