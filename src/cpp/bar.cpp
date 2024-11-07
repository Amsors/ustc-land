#include "game/widgets/bar.h"

#include "i18n.h"
#include "game/main_application.h"
#include "game/widgets/label.h"
#include "game/widgets/tabwidget.h"
#include "GLFW/glfw3.h"
#include "nanogui/button.h"
#include "nanogui/label.h"
#include "nanogui/layout.h"
#include "nanogui/screen.h"
#include "nanogui/tabwidget.h"
#include "spdlog/spdlog.h"

Bar::Bar(nanogui::Widget *parent, const double moveSpeed, const SizeProvider &width, const SizeProvider &height, const SizeProvider &margin, const DistanceProvider &distance0, const DistanceProvider &maxDistance):
    nanogui::Window(parent, ""), MOVE_SPEED(moveSpeed),
    WIDTH(width(screen())), HEIGHT(height(screen())), MARGIN(margin(screen())),
    MAX_DISTANCE(maxDistance(WIDTH, HEIGHT, MARGIN)), distance(distance0(WIDTH, HEIGHT, MARGIN)), stroke() {
    m_theme->m_window_fill_focused = BAR_BACKGROUND;
    m_theme->m_window_fill_unfocused = BAR_BACKGROUND;
    m_theme->m_text_color = BAR_TEXT;

    NVGcontext *ctx = screen()->nvg_context();
    int strokeImage = nvgCreateImage(ctx, "textures/bound_stroke.png", NVG_IMAGE_REPEATX | NVG_IMAGE_REPEATY);
    int imageWidth, imageHeight;
    nvgImageSize(ctx, strokeImage, &imageWidth, &imageHeight);
    stroke = nvgImagePattern(ctx, 0, 0, imageWidth, imageHeight, 0, strokeImage, 1.f);
}

// 禁用拖动操作
bool Bar::mouse_drag_event(const nanogui::Vector2i &p, const nanogui::Vector2i &rel, int button, int modifiers) {
    return true;
}

void Bar::draw(NVGcontext *ctx) {
    Window::draw(ctx);

    // TODO: 采用随机笔刷来绘制更好看的边框
    nvgSave(ctx);
    nvgStrokePaint(ctx, stroke);
    nvgStrokeWidth(ctx, 5.f);
    nvgBeginPath(ctx);
    nvgRect(ctx, m_pos.x() - .5f, m_pos.y() - .5f, m_size.x() + 1.f, m_size.y() + 1.f);
    nvgStroke(ctx);
    nvgRestore(ctx);
}

WelcomeBar::WelcomeBar(nanogui::Widget *parent, const double moveSpeed):
    Bar(parent, moveSpeed,
        [](const nanogui::Screen *const screen)-> int {
            return static_cast<int>(screen->size().x() * .2f);
        }, [](const nanogui::Screen *const screen)-> int {
            return static_cast<int>(screen->size().y() * .4f);
        }, [](const nanogui::Screen *const) -> int {
            return 10;
        }, [](const int, const int, const int) -> double {
            return 0.;
        }, [](const int, const int height, const int margin) -> double {
            return margin * 2 + height;
        }) {
    set_fixed_size({WIDTH, HEIGHT});
    x = MARGIN;
    y = static_cast<int>(screen()->size().y()) - MARGIN - HEIGHT;
    set_position({static_cast<int>(x), static_cast<int>(y)});
    set_layout(new nanogui::BoxLayout(nanogui::Orientation::Vertical, nanogui::Alignment::Middle, MARGIN, 20));

    new nanogui::Label(this, "USTC Land", "hyswhand", HEIGHT * .2f);
    new ClickableLabel(this, i18n::translated("screen.welcome.start"), "msyh", "msyhbd", HEIGHT / 10,
        [this](const nanogui::Vector2i &, int, bool, int) -> bool {
            if(auto *const app = dynamic_cast<MainApplication*>(screen())) {
                app->startGame();
                return true;
            }
            return false;
        });
    new ClickableLabel(this, i18n::translated("screen.welcome.quit"), "msyh", "msyhbd", HEIGHT / 10,
        [this](const nanogui::Vector2i &, int, bool, int) -> bool {
            screen()->set_visible(false);
            return true;
        }
    );
}

void WelcomeBar::move(const double deltaTime, const bool isLeft) {
    if(isLeft && distance < MAX_DISTANCE) {
        y += std::min(deltaTime * MOVE_SPEED, MAX_DISTANCE - distance);
        set_position({static_cast<int>(x), static_cast<int>(y)});
        distance = std::min(MAX_DISTANCE, distance + deltaTime * MOVE_SPEED);
    } else if(!isLeft && distance > 0) {
        y -= std::min(deltaTime * MOVE_SPEED, distance);
        set_position({static_cast<int>(x), static_cast<int>(y)});
        distance = std::max(0., distance - deltaTime * MOVE_SPEED);
    }
}

ListBar::ListBar(nanogui::Widget *parent, const double moveSpeed):
    Bar(parent, moveSpeed,
        [](const nanogui::Screen *const screen)-> int {
            return static_cast<int>(screen->size().x() * .2f);
        }, [](const nanogui::Screen *const screen)-> int {
            return static_cast<int>(screen->size().y() * .75f) - 15;
        }, [](const nanogui::Screen *const) -> int {
            return 10;
        }, [](const int width, const int, const int margin) -> double {
            return margin * 2 + width;
        }, [](const int width, const int, const int margin) -> double {
            return margin * 2 + width;
        }) {
    set_fixed_size({WIDTH, HEIGHT});
    x = -MARGIN - WIDTH;
    y = MARGIN;
    set_position({static_cast<int>(x), static_cast<int>(y)});
    set_layout(new nanogui::GroupLayout(MARGIN));

    auto *tab = new StyledTabWidget(this, stroke, "msyhbd");
    tab->set_font_size(20);
    auto *list1 = new nanogui::Widget(tab);
    auto *list2 = new nanogui::Widget(tab);
    tab->append_tab("list1", list1);
    tab->append_tab("list2", list2);
}

void ListBar::move(const double deltaTime, const bool isLeft) {
    if(isLeft && distance < MAX_DISTANCE) {
        x -= std::min(deltaTime * MOVE_SPEED, MAX_DISTANCE - distance);
        set_position({static_cast<int>(x), static_cast<int>(y)});
        distance = std::min(MAX_DISTANCE, distance + deltaTime * MOVE_SPEED);
    } else if(!isLeft && distance > 0) {
        x += std::min(deltaTime * MOVE_SPEED, distance);
        set_position({static_cast<int>(x), static_cast<int>(y)});
        distance = std::max(0., distance - deltaTime * MOVE_SPEED);
    }
}

InfoBar::InfoBar(nanogui::Widget *parent, const double moveSpeed):
    Bar(parent, moveSpeed,
        [](const nanogui::Screen *const screen)-> int {
            return static_cast<int>(screen->size().x() * .2f);
        }, [](const nanogui::Screen *const screen)-> int {
            return static_cast<int>(screen->size().y() * .25f) - 15;
        }, [](const nanogui::Screen *const) -> int {
            return 10;
        }, [](const int width, const int, const int margin) -> double {
            return margin * 2 + width;
        }, [](const int width, const int, const int margin) -> double {
            return margin * 2 + width;
        }) {
    set_fixed_size({WIDTH, HEIGHT});
    x = -MARGIN - WIDTH;
    y = static_cast<int>(screen()->size().y()) - MARGIN - HEIGHT;
    set_position({static_cast<int>(x), static_cast<int>(y)});
}

void InfoBar::move(const double deltaTime, const bool isLeft) {
    if(isLeft && distance < MAX_DISTANCE) {
        x -= std::min(deltaTime * MOVE_SPEED, MAX_DISTANCE - distance);
        set_position({static_cast<int>(x), static_cast<int>(y)});
        distance = std::min(MAX_DISTANCE, distance + deltaTime * MOVE_SPEED);
    } else if(!isLeft && distance > 0) {
        x += std::min(deltaTime * MOVE_SPEED, distance);
        set_position({static_cast<int>(x), static_cast<int>(y)});
        distance = std::max(0., distance - deltaTime * MOVE_SPEED);
    }
}

void InfoBar::draw(NVGcontext *ctx) {
    Bar::draw(ctx);

    nvgSave(ctx);

    static constexpr int FONT_SIZE = 20;
    static constexpr int SCROLL_SPEED = 20;
    static const std::string &text = i18n::translated("text.render.very_long");
    static NVGtextRow rows[100];
    nvgFontSize(ctx, FONT_SIZE);
    nvgFontFace(ctx, "msyh");
    nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    static const int LINES = nvgTextBreakLines(ctx, text.c_str(), nullptr, WIDTH - MARGIN * 2, rows, 100);
    static const int TEXT_HEIGHT = FONT_SIZE * LINES;
    const int x0 = m_pos.x() + MARGIN;
    const int y0 = m_pos.y() + MARGIN -
        (dynamic_cast<MainApplication*>(screen())->gameState() == MainApplication::PLAYING ? static_cast<int>(glfwGetTime() * SCROLL_SPEED) % TEXT_HEIGHT : 0);
    nvgIntersectScissor(ctx, m_pos.x() + MARGIN, m_pos.y() + MARGIN, WIDTH - MARGIN * 2, HEIGHT - MARGIN * 2);
    for(int i = 0, y = y0; i < LINES; i++, y += FONT_SIZE) {
        nvgText(ctx, x0, y, rows[i].start, rows[i].end);
    }

    nvgRestore(ctx);
}
