#include "game/widgets/bar.h"

#include "i18n.h"
#include "game/common.h"
#include "game/widgets/label.h"
#include "nanogui/button.h"
#include "nanogui/label.h"
#include "nanogui/layout.h"
#include "nanogui/screen.h"
#include "spdlog/spdlog.h"

const nanogui::Color BAR_BACKGROUND(0xFF, 0xF9, 0xE3, 0xFF);
const nanogui::Color BAR_TEXT(0x00, 0x00, 0x00, 0xFF);

Bar::Bar(nanogui::Widget *parent):
    nanogui::Window(parent, ""), stroke() {
    m_theme->m_window_fill_focused = BAR_BACKGROUND;
    m_theme->m_window_fill_unfocused = BAR_BACKGROUND;
    m_theme->m_text_color = BAR_TEXT;

    NVGcontext *ctx = screen()->nvg_context();
    int strokeImage = nvgCreateImage(ctx, "textures/bound_stroke.png", NVG_IMAGE_REPEATX | NVG_IMAGE_REPEATY);
    int width, height;
    nvgImageSize(ctx, strokeImage, &width, &height);
    stroke = nvgImagePattern(ctx, 0, 0, width, height, 0, strokeImage, 1.f);
}

// 禁用拖动操作
bool Bar::mouse_drag_event(const nanogui::Vector2i &p, const nanogui::Vector2i &rel, int button, int modifiers) {
    return true;
}

void Bar::draw(NVGcontext *ctx) {
    Window::draw(ctx);

    // TODO: 采用随机笔刷来绘制更好看的边框
    nvgStrokePaint(ctx, stroke);
    nvgStrokeWidth(ctx, 5.f);
    nvgBeginPath(ctx);
    nvgRect(ctx, m_pos.x() - .5f, m_pos.y() - .5f, m_size.x() + 1.f, m_size.y() + 1.f);
    nvgStroke(ctx);
    nvgRestore(ctx);
}

WelcomeBar::WelcomeBar(nanogui::Widget *parent):
    Bar(parent) {
    const int WIDTH = static_cast<int>(screen()->size().x() / screen()->pixel_ratio()) / 5;
    const int HEIGHT = static_cast<int>(screen()->size().y() / screen()->pixel_ratio()) * 2 / 5;
    constexpr int MARGIN = 10;
    set_fixed_size({WIDTH, HEIGHT});
    set_position({MARGIN, static_cast<int>(screen()->size().y() / screen()->pixel_ratio()) - MARGIN - HEIGHT});
    set_layout(new nanogui::BoxLayout(nanogui::Orientation::Vertical, nanogui::Alignment::Middle, MARGIN, 20));

    new nanogui::Label(this, "USTC Land", "hyswhand", HEIGHT / 5);
    new ClickableLabel(this, i18n::translated("screen.welcome.start"), "msyh", "msyhbd", HEIGHT / 10);
    new ClickableLabel(this, i18n::translated("screen.welcome.quit"), "msyh", "msyhbd", HEIGHT / 10,
        [this](const nanogui::Vector2i &, int, bool, int) -> bool {
            screen()->set_visible(false);
            return true;
        }
    );
}
