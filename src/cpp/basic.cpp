#include "game/logic/basic.h"

const float Card::W = 5;
const float Card::H = 7;

Card::Card(const nanogui::Vector3f &pos) {
    this->pos = pos;
}

void Card::calc(const double deltaTime) {}
