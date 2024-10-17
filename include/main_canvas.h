#pragma once
#include <nanogui/canvas.h>
#include <nanogui/shader.h>

class MainCanvas final: public nanogui::Canvas {
public:
    explicit MainCanvas(nanogui::Widget *parent);
    void setRotation(float rotation);
    void draw_contents() override;

private:
    nanogui::Shader *shader;
    float rotation;
};
