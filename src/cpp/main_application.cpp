#include "game/main_application.h"

#include "game/common.h"
#include "GLFW/glfw3.h"
#include "nanogui/button.h"
#include "nanogui/layout.h"
#include "nanogui/renderpass.h"
#include "nanogui/shader.h"
#include "nanovg/nanovg.h"
#include "nanovg/stb_image.h"
#include "spdlog/spdlog.h"
#include "game/logic/registry.h"
#include "game/random.h"

MainApplication::MainApplication():
    nanogui::Screen({reg.gameSettings.pixel_x, reg.gameSettings.pixel_y}, "USTC Land", false, reg.gameSettings.full_screen), camera(0, -4, -40) {
    SPDLOG_LOGGER_TRACE(spdlog::get("render"), "Main window created! size of the window: (x: {}, y: {}); pixel ratio: {:.2}", m_size.x(), m_size.y(), m_pixel_ratio);

    // 加载字体
    nvgCreateFont(m_nvg_context, "msyh", R"(fonts\msyh.ttf)");
    nvgCreateFont(m_nvg_context, "msyhbd", R"(fonts\msyhbd.ttf)");
    nvgCreateFont(m_nvg_context, "hyswhand", R"(fonts\hyswhand.ttf)");

    // 加载GUI
    if(m_fullscreen) {
        m_size.x() /= m_pixel_ratio;
        m_size.y() /= m_pixel_ratio;
    }
    welcomeBar = new WelcomeBar(this, 500.);
    listBar = new ListBar(this, 440.);
    infoBar = new InfoBar(this, 440.);
    perform_layout();

    std::string bvs, bfs, cvs, cfs, svs, sfs, fvs, ffs;
    loadShaders("background", bvs, bfs);
    loadShaders("card", cvs, cfs);
    loadShaders("shadow", svs, sfs);
    loadShaders("floor", fvs, ffs);
    renderPass = new nanogui::RenderPass({this});
    renderPass->set_clear_color(0, {0, 0, 0, 0});
    cardShader = new nanogui::Shader(renderPass, "card", cvs, cfs, nanogui::Shader::BlendMode::AlphaBlend);
    floorShader = new nanogui::Shader(renderPass, "floor", fvs, ffs, nanogui::Shader::BlendMode::AlphaBlend);

    bgTex = new nanogui::Texture(nanogui::Texture::PixelFormat::RGBA, nanogui::Texture::ComponentFormat::UInt8, m_size, nanogui::Texture::InterpolationMode::Bilinear, nanogui::Texture::InterpolationMode::Bilinear, nanogui::Texture::WrapMode::Repeat, 1, nanogui::Texture::TextureFlags::ShaderRead | nanogui::Texture::TextureFlags::RenderTarget);
    bgPass = new nanogui::RenderPass({bgTex});
    bgPass->set_clear_color(0, {0x85, 0xA8, 0x87, 0xFF});
    bgShader = new nanogui::Shader(bgPass, "background", bvs, bfs, nanogui::Shader::BlendMode::AlphaBlend);

    depthMap = new nanogui::Texture(nanogui::Texture::PixelFormat::RGBA, nanogui::Texture::ComponentFormat::UInt8, m_size, nanogui::Texture::InterpolationMode::Bilinear, nanogui::Texture::InterpolationMode::Bilinear, nanogui::Texture::WrapMode::Repeat, 1, nanogui::Texture::TextureFlags::ShaderRead | nanogui::Texture::TextureFlags::RenderTarget);
    shadowPass = new nanogui::RenderPass({depthMap.get()});
    shadowPass->set_clear_color(0, {0xFF, 0xFF, 0xFF, 0xFF});
    shadowShader = new nanogui::Shader(shadowPass, "shadow", svs, sfs, nanogui::Shader::BlendMode::None);

    for(int i = 1; i <= 13; i++) {
        nanogui::Vector2i size;
        int channels;
        const std::string &imageFile = fmt::format("textures/background/{}.png", i);
        const ImageData texture_data(stbi_load(imageFile.c_str(), &size.x(), &size.y(), &channels, 0), stbi_image_free);
        #ifndef __USTC_LAND_RELEASE__
        assert(channels == 4);
        #endif
        auto *tex = new nanogui::Texture(nanogui::Texture::PixelFormat::RGBA, nanogui::Texture::ComponentFormat::UInt8, size, nanogui::Texture::InterpolationMode::Bilinear, nanogui::Texture::InterpolationMode::Bilinear);
        tex->upload(texture_data.get());
        bgTextures.emplace_back(tex);
    }

    constexpr uint32_t indices[] = {
        0, 1, 2, 0, 2, 3
    };
    constexpr float positions[] = {
        0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f
    };
    constexpr float floorPositions[] = {
        -40.f, -20.f, 1.f, -40.f, 20.f, 1.f, 40.f, 20.f, 1.f, 40.f, -20.f, 1.f
    };
    constexpr float texCoords[] = {
        1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f
    };
    constexpr float he = (Card::W - Card::TS) / (2 * Card::TS), ve = (Card::H - Card::TS) / (2 * Card::TS);
    constexpr float cardTexCoords[] = {
        1 + he, -ve, 1 + he, 1 + ve, -he, 1 + ve, -he, -ve
    };

    const nanogui::Matrix4f vp =
        nanogui::Matrix4f::perspective(.78539816f, 10.f, 60.f, m_size.x() * 1.f / m_size.y())
        * nanogui::Matrix4f::look_at(camera, nanogui::Vector3f(0, 0, 0), nanogui::Vector3f(0, 1, 0));
    const nanogui::Matrix4f shadowVp =
        nanogui::Matrix4f::perspective(.90757121f, 10.f, 60.f, depthMap->size().x() * 1.f / depthMap->size().y())
        * nanogui::Matrix4f::look_at({12, -5, -40}, nanogui::Vector3f(0, 0, 0), nanogui::Vector3f(0, 1, 0));
    iVp = nanogui::Matrix4f::inverse(vp);
    SPDLOG_LOGGER_TRACE(spdlog::get("main"), "\nvp:\n{}\niVp:\n{}", vp, iVp);
    SPDLOG_LOGGER_TRACE(spdlog::get("main"), "\nvp:\n{}", shadowVp);

    bgShader->set_buffer("indices", nanogui::VariableType::UInt32, {3 * 2}, indices);
    bgShader->set_buffer("positions", nanogui::VariableType::Float32, {4, 3}, positions);
    bgShader->set_buffer("texCoords", nanogui::VariableType::Float32, {4, 2}, texCoords);
    bgShader->set_uniform("bgColor", bgPass->clear_color(0));
    bgShader->set_uniform("vp", vp);

    cardShader->set_buffer("indices", nanogui::VariableType::UInt32, {3 * 2}, indices);
    cardShader->set_buffer("positions", nanogui::VariableType::Float32, {4, 3}, positions);
    cardShader->set_buffer("texCoords", nanogui::VariableType::Float32, {4, 2}, cardTexCoords);
    cardShader->set_uniform("vp", vp);
    cardShader->set_uniform("shadowVp", shadowVp);
    cardShader->set_uniform("iVp", iVp);
    cardShader->set_uniform("windowSize", nanogui::Vector2f(m_size.x(), m_size.y()) * m_pixel_ratio);
    cardShader->set_uniform("cardSize", nanogui::Vector3f(Card::W, Card::H, Card::R));
    cardShader->set_texture("depthMap", depthMap.get());

    floorShader->set_buffer("indices", nanogui::VariableType::UInt32, {3 * 2}, indices);
    floorShader->set_buffer("positions", nanogui::VariableType::Float32, {4, 3}, floorPositions);
    floorShader->set_uniform("vp", vp);
    floorShader->set_uniform("shadowVp", shadowVp);
    floorShader->set_uniform("iVp", iVp);
    floorShader->set_uniform("windowSize", nanogui::Vector2f(m_size.x(), m_size.y()) * m_pixel_ratio);
    floorShader->set_texture("depthMap", depthMap.get());
    floorShader->set_texture("bgTex", bgTex.get());

    shadowShader->set_buffer("indices", nanogui::VariableType::UInt32, {3 * 2}, indices);
    shadowShader->set_buffer("positions", nanogui::VariableType::Float32, {4, 3}, positions);
    shadowShader->set_uniform("vp", shadowVp);
    shadowShader->set_uniform("iVp", nanogui::Matrix4f::inverse(shadowVp));
    shadowShader->set_uniform("windowSize", nanogui::Vector2f(m_size.x(), m_size.y()));
    shadowShader->set_uniform("cardSize", nanogui::Vector3f(Card::W, Card::H, Card::R));
}

MainApplication::~MainApplication() {
    delete bgShader;
    delete cardShader;
    delete floorShader;
    delete renderPass;
    delete bgPass;
    delete shadowPass;
}

bool MainApplication::keyboard_event(const int key, const int scancode, const int action, const int modifiers) {
    if(Screen::keyboard_event(key, scancode, action, modifiers)) {
        return true;
    }
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if(state == PLAYING) {
            quitGame();
        } else {
            set_visible(false);
        }
        return true;
    }
    return false;
}

bool MainApplication::mouse_button_event(const nanogui::Vector2i &p, const int button, const bool down, const int modifiers) {
    if(Screen::mouse_button_event(p, button, down, modifiers)) {
        return true;
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
        if(down) {
            mouseState = RIGHT;
            if(state == PLAYING) {
                if (reg.gameSettings.cheat == true) {          
                    for (auto it = reg.cardAttained.begin(); it != reg.cardAttained.end(); it++) {
                        if (it->second == false) {
                            newCards.emplace(it->first);
                            break;
                        }
                    }
                }
                return true;
            }
        } else {
            mouseState = NONE;
        }
    }
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
        if(down) {
            mouseState = LEFT;
            const nanogui::Vector2f cursor = screenToWorldZ0(p);
            for(int i = 0; i < stacks.size(); i++) {
                for(auto ritCard = stacks[i].cards.rbegin(); ritCard != stacks[i].cards.rend(); ++ritCard) {
                    if((*ritCard)->contains(cursor)) {
                        if(ritCard != stacks[i].cards.rend() - 1) {
                            std::vector sub(ritCard.base() - 1, stacks[i].cards.end());//取走的牌
                            stacks[i].cards.erase(ritCard.base() - 1, stacks[i].cards.end());//剩余的牌

                            stacks[i].initial();
                            stacks[i].stamp = stamp;
                            stamp++;

                            if(stacks[i].cards.empty()) {
                                stacks.erase(stacks.begin() + i);
                            }
                            Stack tmp = Stack(sub, UNCHECKED, stamp);
                            stacks.emplace_back(tmp);
                            stamp++;
                            movingStack = true;
                        } else {
                            // 确保选中的牌堆是最后渲染出来的（即永远在最上方）
                            std::swap(stacks[i], stacks.back());
                        }
                        for(const auto &card: stacks.back().cards) {
                            card->move({0.f, 0.f, -1.f});
                        }
                        movingStack = true;
                        return true;
                    }
                }
            }
        } else {
            mouseState = NONE;
            if(movingStack) {
                for(const auto &movingCard: stacks.back().cards) {
                    movingCard->move({0.f, 0.f, 1.f});
                }
                // 尝试合并两堆卡牌
                const nanogui::Vector3f &pos = stacks.back().cards.back()->getPosition();
                const float chosenH = stacks.back().cards[0]->getPosition().y() + Card::H - pos.y();
                for(auto it = stacks.begin(); it != stacks.end() - 1; ++it) {
                    const nanogui::Vector3f &pos2 = it->cards.back()->getPosition();
                    if(std::abs(pos2.x() - pos.x()) <= Card::W &&
                        std::abs(pos2.y() - pos.y()) <= std::min(chosenH, (*it).cards[0]->getPosition().y() + Card::H - pos2.y())) {
                        for(int j = 0; j < stacks.back().cards.size(); j++) {
                            stacks.back().cards[j]->moveTo({pos2.x(), pos2.y() - Card::D * (j + 1)});
                        }
                        it->cards.insert(it->cards.end(), stacks.back().cards.begin(), stacks.back().cards.end());
                        it->status = UNCHECKED;
                        it->stamp = stamp;
                        stamp++;

                        stacks.erase(stacks.end() - 1);
                        break;
                    }
                }
                movingStack = false;
                return true;
            }
        }
    }
    return false;
}

bool MainApplication::mouse_motion_event(const nanogui::Vector2i &p, const nanogui::Vector2i &rel, const int button, const int modifiers) {
    if(Screen::mouse_motion_event(p, rel, button, modifiers)) {
        return true;
    }
    switch(mouseState) {
        case LEFT:
            if(movingStack) {
                const nanogui::Vector2f delta = screenToWorldZ0(p) - screenToWorldZ0(p - rel);
                for(const auto &card: stacks.back().cards) {
                    card->move({delta[0], delta[1], 0.f});
                }
            }
            break;
        default:
            break;
    }
    return false;
}

void MainApplication::draw(NVGcontext *ctx) {
    Screen::draw(ctx);
}

void MainApplication::draw_contents() {
    // 生成ShadowMap
    shadowPass->resize(depthMap->size());
    shadowPass->begin();
    for(const auto &stack: stacks) {
        for(const auto &card: stack.cards) {
            nanogui::Matrix4f model =
                nanogui::Matrix4f::translate(card->getPosition()) *
                nanogui::Matrix4f::scale({Card::W, Card::H, 1.f});
            shadowShader->set_uniform("model", model);
            shadowShader->begin();
            shadowShader->draw_array(nanogui::Shader::PrimitiveType::Triangle, 0, 6, true);
            shadowShader->end();
        }
    }
    shadowPass->end();

    // 绘制背景
    bgPass->begin();
    bgShader->set_uniform("glfwTime", static_cast<float>(glfwGetTime()));
    for(const auto &[pos, i]: BG_TEXTURE_POSITIONS) {
        nanogui::Matrix4f model =
            nanogui::Matrix4f::translate({pos.x(), pos.y(), 0}) *
            nanogui::Matrix4f::scale({bgTextures[i]->size().x() / 33.f, bgTextures[i]->size().y() / 33.f, 1.f});
        bgShader->set_uniform("model", model);
        bgShader->set_texture("tex", bgTextures[i].get());
        bgShader->begin();
        bgShader->draw_array(nanogui::Shader::PrimitiveType::Triangle, 0, 6, true);
        bgShader->end();
    }
    bgPass->end();
    renderPass->resize(framebuffer_size());
    renderPass->begin();
    floorShader->begin();
    floorShader->draw_array(nanogui::Shader::PrimitiveType::Triangle, 0, 6, true);
    floorShader->end();

    // 处理游戏内的时间变化
    const double now = glfwGetTime();
    const double deltaTime = now - lastFrame;
    lastFrame = now;

    switch(state) {
        case PREPARING:
            if(welcomeBar->where() == Bar::THERE) {
                listBar->move(deltaTime, false);
                infoBar->move(deltaTime, false);
                if(listBar->where() == Bar::HERE/* && infoBar->where() == Bar::HERE*/) {
                    state = PLAYING;
                    this->logicInit();
                }
            } else {
                welcomeBar->move(deltaTime, true);
            }
            break;
        case QUITTING:
            if(listBar->where() == Bar::THERE/* && infoBar->where() == Bar::THERE*/) {
                welcomeBar->move(deltaTime, false);
                if(welcomeBar->where() == Bar::HERE) {
                    state = TITLE;
                }
            } else {
                listBar->move(deltaTime, true);
                infoBar->move(deltaTime, true);
            }
            break;
        case PLAYING:

            checkCard();
            processWaitingCard();
            giveReward();
            //updateAdvancement();
            //检查成就在givereward里完成
            addCard();

            for(const auto &stack: stacks) {
                for(const auto &card: stack.cards) {
                    card->calc(deltaTime);
                }
            }
        // TODO: 绘制前景（卡牌、特效等）
            for(const auto &stack: stacks) {
                for(const auto &card: stack.cards) {
                    nanogui::Matrix4f model =
                        nanogui::Matrix4f::translate(card->getPosition()) *
                        nanogui::Matrix4f::scale({Card::W, Card::H, 1.f});
                    cardShader->set_uniform("model", model);
                    cardShader->set_uniform("cardColor", card->getColor());
                    cardShader->set_texture("tex", card->getTexture().get());
                    cardShader->begin();
                    cardShader->draw_array(nanogui::Shader::PrimitiveType::Triangle, 0, 6, true);
                    cardShader->end();
                }
            }
            /*checkCard();
            processWaitingCard();
            giveReward();
            addCard();*/
            break;
        default:
            break;
    }

    renderPass->end();
}

void MainApplication::startGame() {
    state = PREPARING;
}

void MainApplication::quitGame() {
    state = QUITTING;
    // 清空卡牌列表，同时调用卡牌的析构函数，进行保存
    stacks.clear();
}
