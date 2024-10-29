#include "game/widgets/tabwidget.h"

#include "game/common.h"

StyledTabWidget::StyledTabWidget(nanogui::Widget *parent, const NVGpaint &stroke, const std::string &font):
    nanogui::TabWidget(parent, font), stroke(stroke) {
    theme()->m_button_gradient_top_pushed = BAR_BACKGROUND;
    theme()->m_button_gradient_bot_pushed = BAR_BACKGROUND;
    theme()->m_border_dark = BAR_BACKGROUND;
    theme()->m_border_light = BAR_BACKGROUND;
    theme()->m_border_medium = BAR_BACKGROUND;
}

void StyledTabWidget::draw(NVGcontext *ctx) {
    TabWidget::draw(ctx);

    // TODO: 采用随机笔刷来绘制更好看的边框
    const int tab_height = font_size() + 2 * m_theme->m_tab_button_vertical_padding;
    nvgSave(ctx);
    nvgStrokePaint(ctx, stroke);
    nvgStrokeWidth(ctx, 3.f);
    nvgBeginPath(ctx);
    nvgMoveTo(ctx, m_pos.x(), m_pos.y() + tab_height + 4);
    nvgLineTo(ctx, m_pos.x() + m_tab_offsets[m_active_tab] + 3.f, m_pos.y() + tab_height + 4);
    nvgLineTo(ctx, m_pos.x() + m_tab_offsets[m_active_tab] + 3.f, m_pos.y() + 1.5f);
    nvgLineTo(ctx, m_pos.x() + m_tab_offsets[m_active_tab + 1], m_pos.y() + 1.5f);
    nvgLineTo(ctx, m_pos.x() + m_tab_offsets[m_active_tab + 1], m_pos.y() + tab_height + 4);
    nvgLineTo(ctx, m_pos.x() + m_size.x(), m_pos.y() + tab_height + 4);
    nvgStroke(ctx);
    nvgRestore(ctx);
}
