#include "game/widgets/bar.h"

#include <ranges>

#include "i18n.h"
#include "game/main_application.h"
#include "game/logic/registry.h"
#include "game/widgets/button.h"
#include "game/widgets/label.h"
#include "game/widgets/tabwidget.h"
#include "GLFW/glfw3.h"
#include "nanogui/button.h"
#include "nanogui/icons.h"
#include "nanogui/imageview.h"
#include "nanogui/label.h"
#include "nanogui/layout.h"
#include "nanogui/screen.h"
#include "nanogui/tabwidget.h"
#include "nanovg/nanovg.h"
#include "nanovg/stb_image.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bundled/xchar.h"

Bar::Bar(nanogui::Widget *parent, const double moveSpeed, const SizeProvider &width, const SizeProvider &height, const SizeProvider &margin, const DistanceProvider &distance0, const DistanceProvider &maxDistance):
    nanogui::Window(parent, ""), MOVE_SPEED(moveSpeed),
    WIDTH(width(screen())), HEIGHT(height(screen())), MARGIN(margin(screen())),
    MAX_DISTANCE(maxDistance(WIDTH, HEIGHT, MARGIN)), distance(distance0(WIDTH, HEIGHT, MARGIN)) {
    m_theme->m_window_fill_focused = BAR_BACKGROUND;
    m_theme->m_window_fill_unfocused = BAR_BACKGROUND;
    m_theme->m_text_color = BAR_TEXT;
}

// 禁用拖动操作
bool Bar::mouse_drag_event(const nanogui::Vector2i &p, const nanogui::Vector2i &rel, int button, int modifiers) {
    return true;
}

void Bar::draw(NVGcontext *ctx) {
    Window::draw(ctx);

    nvgSave(ctx);
    nvgStrokeWidth(ctx, 8.f);
    nvgStrokeColor(ctx, nvgRGB(0, 0, 0));
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
            return screen->size().y() - 20;
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
    set_layout(new nanogui::GroupLayout(0));

    auto *tab = new StyledTabWidget(this, "msyh", "msyhbd");
    tab->set_font_size(HEIGHT / 20);
    attribute = new nanogui::Widget(tab);
    advancement = new nanogui::Widget(tab);
    auto *layout = new nanogui::GroupLayout(5, 2);
    attribute->set_layout(layout);
    advancement->set_layout(layout);
    tab->append_tab(i18n::translated("tab.title.attribute"), attribute);
    tab->append_tab(i18n::translated("tab.title.advancement"), advancement);
    tab->set_callback([](int) {});

    theme()->m_button_gradient_bot_pushed = theme()->m_button_gradient_top_pushed = BAR_BACKGROUND;
    theme()->m_border_dark = theme()->m_border_light = theme()->m_border_medium = BAR_BACKGROUND;
    theme()->m_text_color_shadow = BAR_BACKGROUND;

    updateInfo();
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

void ListBar::updateInfo() {
    while(!attribute->children().empty()) {
        attribute->remove_child_at(0);
    }
    while(!advancement->children().empty()) {
        advancement->remove_child_at(0);
    }

    nanogui::Button *btn;
    for(const auto &anAttr: reg.regAttribute | std::views::values) {
        if(anAttr->getIsArray()) {
            btn = new CustomButton(attribute, anAttr->getName() + ": ", FA_CIRCLE);
            btn->set_background_color(BAR_BACKGROUND);
            btn->set_icon_position(nanogui::Button::IconPosition::Left);
            for(const auto &value: anAttr->getAttributeArray() | std::views::values) {
                btn = new CustomButton(attribute, fmt::format("{}: {}", anAttr->getName(), value), FA_MINUS);
                btn->set_background_color(BAR_BACKGROUND);
                btn->set_icon_position(nanogui::Button::IconPosition::Left);
            }
        } else {
            btn = new CustomButton(attribute, fmt::format("{}: {}", anAttr->getName(), anAttr->getAttributeValue()), FA_CIRCLE);
            btn->set_background_color(BAR_BACKGROUND);
            btn->set_icon_position(nanogui::Button::IconPosition::Left);
        }
    }
    for(const auto &anAdv: reg.regAdvancement | std::views::values) {
        btn = new CustomButton(advancement, anAdv->getName(), anAdv->getEstablished() ? FA_CHECK_SQUARE : FA_SQUARE);
        btn->set_background_color(BAR_BACKGROUND);
    }

    perform_layout(screen()->nvg_context());
}
