#include "game/widgets/button.h"

#include "nanogui/opengl.h"

void CustomButton::draw(NVGcontext *ctx) {
    const int font_size = m_font_size == -1 ? m_theme->m_button_font_size : m_font_size;
    nanogui::Vector2f text_pos(m_pos.x(), m_pos.y() + m_size.y() / 2.0f);
    constexpr NVGcolor text_color = {0.f, 0.f, 0.f, 1.f};
    const auto icon = nanogui::utf8(m_icon);

    const float ih = font_size * icon_scale();
    const float iw = nvgTextBounds(ctx, 0, 0, icon.data(), nullptr, nullptr);
    nvgFontSize(ctx, ih);
    nvgFontFace(ctx, "icons");
    nvgFillColor(ctx, text_color);
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgText(ctx, text_pos.x(), text_pos.y(), icon.data(), nullptr);

    nvgFontSize(ctx, font_size);
    nvgFontFace(ctx, "msyh");
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgFillColor(ctx, text_color);
    nvgText(ctx, text_pos.x() + 12 + iw, text_pos.y(), text.c_str(), nullptr);
}
