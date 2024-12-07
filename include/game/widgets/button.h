#pragma once

#include "nanogui/button.h"
#include "nanogui/opengl.h"

class CustomButton final: public nanogui::Button {
public:
    CustomButton(Widget *parent, std::string caption, const int icon):
        Button(parent, "", icon), text(std::move(caption)) {
        #ifndef __USTC_LAND_RELEASE__
        assert(nanogui::nvg_is_font_icon(icon));
        #endif
    }

    void draw(NVGcontext *ctx) override;

private:
    const std::string text;
};
