#include "game/logic/basic.h"

const float Card::W = 5.f;
const float Card::H = 7.f;
const float Card::D = 1.f;
const float Card::R = .6f;

Card::Card(const nanogui::Vector3f &pos) {
    this->pos = pos;
    color = nanogui::Color(rand() % 256 / 256.f, rand() % 256 / 256.f, rand() % 256 / 256.f, 1.f);
}

void Card::calc(const double deltaTime) {}
