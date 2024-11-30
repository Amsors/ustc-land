#include "game/logic/basic.h"

#include "nanogui/texture.h"
#include "nanovg/stb_image.h"
#include "game/logic/register.h"

//Card::Card(const Type type, const std::string &name, const nanogui::Vector3f &pos):
//    pos(pos), name(name), type(type), color(colorSetter(type)) {
//    nanogui::Vector2i size;
//    int channels;
//    const ImageData texture_data(stbi_load(("textures" + getTexturePath(name)).c_str(), &size.x(), &size.y(), &channels, 0), stbi_image_free);
//    #ifndef __USTC_LAND_RELEASE__
//    assert(channels == 4);
//    #endif
//    auto *tex = new nanogui::Texture(nanogui::Texture::PixelFormat::RGBA, nanogui::Texture::ComponentFormat::UInt8, size, nanogui::Texture::InterpolationMode::Bilinear, nanogui::Texture::InterpolationMode::Bilinear);
//    tex->upload(texture_data.get());
//    texture = tex;
//}

Card::Card(const std::string type, const std::string& name, const nanogui::Vector3f& pos) :
    pos(pos), name(name), type(type), color(reg.cardTypeColor[type]) {
    nanogui::Vector2i size;
    int channels;
    const ImageData texture_data(stbi_load(("textures" + getTexturePath(name)).c_str(), &size.x(), &size.y(), &channels, 0), stbi_image_free);
    #ifndef __USTC_LAND_RELEASE__
    assert(channels == 4);
    #endif
    auto* tex = new nanogui::Texture(nanogui::Texture::PixelFormat::RGBA, nanogui::Texture::ComponentFormat::UInt8, size, nanogui::Texture::InterpolationMode::Bilinear, nanogui::Texture::InterpolationMode::Bilinear);
    tex->upload(texture_data.get());
    texture = tex;
}

void Card::calc(const double deltaTime) {}
