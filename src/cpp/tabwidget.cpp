#include <utility>

#include "game/widgets/tabwidget.h"

#include "game/common.h"
#include "nanogui/button.h"
#include "nanovg/nanovg.h"
#include "spdlog/spdlog.h"

const nanogui::Color StyledTabWidget::UNFOCUSED = nanogui::Color(233, 223, 187, 255);

StyledTabWidget::StyledTabWidget(nanogui::Widget *parent, const std::string &quitTab, std::string enteredTab):
    nanogui::TabWidget(parent, quitTab), enteredTabFont(std::move(enteredTab)), hovering(-1), tabWidth(0) {}

void StyledTabWidget::draw(NVGcontext *ctx) {
    const int tab_height = m_font_size + 2 * m_theme->m_tab_button_vertical_padding;

    nvgSave(ctx);
    nvgFontSize(ctx, m_font_size);
    nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    for(size_t i = 0; i < m_tab_captions.size(); i++) {
        const int x = m_pos.x() + m_tab_offsets[i], y = m_pos.y();
        const int width = m_tab_offsets[i + 1] - m_tab_offsets[i];
        // 绘制未选中标签的背景
        if(i != m_active_tab) {
            nvgBeginPath(ctx);
            nvgRect(ctx, x + 0.5f, y + 1.5f, width, tab_height);
            nvgFillColor(ctx, UNFOCUSED);
            nvgFill(ctx);
        }
        // 绘制文字
        const int tx = x + tabWidth / 2;
        const int ty = y + tab_height / 2 + 2;
        nvgFillColor(ctx, m_theme->m_text_color);
        nvgFontFace(ctx, (hovering == i ? enteredTabFont : m_font).c_str());
        nvgText(ctx, tx, ty, m_tab_captions[i].c_str(), nullptr);
    }
    nvgRestore(ctx);

    if(!m_children.empty()) {
        nvgTranslate(ctx, m_pos.x(), m_pos.y());
        for(const auto child: m_children) {
            if(!child->visible()) {
                continue;
            }
            #if !defined(NANOGUI_SHOW_WIDGET_BOUNDS)
            nvgSave(ctx);
            nvgIntersectScissor(ctx, child->position().x(), child->position().y(), child->size().x(), child->size().y());
            #endif

            child->draw(ctx);

            #if !defined(NANOGUI_SHOW_WIDGET_BOUNDS)
            nvgRestore(ctx);
            #endif
        }
        nvgTranslate(ctx, -m_pos.x(), -m_pos.y());
    }
}

bool StyledTabWidget::mouse_motion_event(const nanogui::Vector2i &p, const nanogui::Vector2i &rel, const int button, const int modifiers) {
    if(TabWidgetBase::mouse_motion_event(p, rel, button, modifiers)) {
        return true;
    }
    hovering = p.y() <= m_pos.y() || p.y() >= m_pos.y() + m_font_size + 2 * m_theme->m_tab_button_vertical_padding ? -1 :
        p.x() <= m_pos.x() || p.x() >= m_pos.x() + m_size.x() ? -1 : (p.x() - m_pos.x()) / tabWidth;
    return true;
}

void StyledTabWidget::perform_layout(NVGcontext *ctx) {
    TabWidget::perform_layout(ctx);

    m_tab_offsets.clear();
    tabWidth = m_size.x() / m_tab_captions.size();
    for(int i = 0; i < m_tab_captions.size(); i++) {
        m_tab_offsets.push_back(tabWidth * i);
    }
    m_tab_offsets.push_back(m_size.x());
}
