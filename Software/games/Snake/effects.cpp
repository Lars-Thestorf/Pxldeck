#include "effects.h"

#define FOOD_COLOR 0x07E0
#define SPEED_COLOR 0xFFE0
#define SLOW_COLOR 0x60CF
#define IMMORTAL_COLOR 0xFFFF
#define BOMB_COLOR 0xF800
#define INVISIBLE_COLOR 0x001F

void Effect::spawn(Point p) {
    point = p;
}
void Effect::render() {
    HLM_graphics *gfx = get_graphics();
    gfx->drawPixel(point.x,point.y,color);
}
bool Effect::isOnPoint(Point p) {
    return arePointsEqual(p, point);
}

Food::Food() {
    color = FOOD_COLOR;
    type = FOOD;
}

Speed::Speed() {
    color = SPEED_COLOR;
    type = SPEED;
}

Slow::Slow() {
    color = SLOW_COLOR;
    type = SLOW;
}

Immortal::Immortal() {
    color = IMMORTAL_COLOR;
    type = IMMORTAL;
}

Bomb::Bomb() {
    color = BOMB_COLOR;
    type = BOMB;
}

Invisible::Invisible() {
    color = INVISIBLE_COLOR;
    type = INVISIBLE;
}
