#pragma once
#include "nanogui/tabwidget.h"
#include "nanovg/nanovg.h"

class StyledTabWidget final: public nanogui::TabWidget {
public:
    StyledTabWidget(nanogui::Widget *parent, const NVGpaint &stroke, const std::string &font = "sans-bold");
    void draw(NVGcontext *ctx) override;

private:
    const NVGpaint &stroke;
};
