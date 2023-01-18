#include "effects.h"

#define foodColor (uint16_t)0x00ff00
#define speedColor (uint16_t)0x00ffff
#define slowColor (uint16_t)0xffff00
#define immortalColor (uint16_t)0xffffff
#define bombColor (uint16_t)0xff0000
#define invisibleColor (uint16_t)0x0000ff

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
    color = foodColor;
    type = FOOD;
}

Speed::Speed() {
    color = speedColor;
    type = SPEED;
}


Slow::Slow() {
    color = slowColor;
    type = SLOW;
}


Immortal::Immortal() {
    color = immortalColor;
    type = IMMORTAL;
}


Bomb::Bomb() {
    color = bombColor;
    type = BOMB;
}


Invisible::Invisible() {
    color = invisibleColor;
    type = INVISIBLE;
}
