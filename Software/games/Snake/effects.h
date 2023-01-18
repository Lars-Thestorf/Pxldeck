#ifndef _LALEMA_EFFECTS_H_
#define _LALEMA_EFFECTS_H_

#include <stdint.h>
#include "point.h"
#include <HLM_graphics.h>


enum EffectType {   FOOD,
                    SPEED,
                    SLOW,
                    IMMORTAL,
                    BOMB,
                    INVISIBLE,
                    NONE };

class Effect {
public:
    EffectType type;
    Effect() {}

    void spawn(Point p);
    void render();
    bool isOnPoint(Point p);

private:
    Point point;
protected:
   uint16_t color;
};


class Food : public Effect {
public:
  Food();
};
class Speed : public Effect {
public:
  Speed();
};
class Slow : public Effect {
public:
  Slow();
};
class Immortal : public Effect {
public:
  Immortal();
};
class Bomb : public Effect {
public:
  Bomb();
};
class Invisible : public Effect {
public:
  Invisible();
};

#endif