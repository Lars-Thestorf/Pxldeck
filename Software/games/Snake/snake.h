#ifndef _LALEMA_SNAKE_H_
#define _LALEMA_SNAKE_H_

#include <stdint.h>
#include <HLM_graphics.h>
#include "point.h"

#define STARTLENGTH_SNAKE 5
#define EFFECT_DURATION 5000
#define UPDATE_INTERVALL_SNAKE_SPEED 150
#define UPDATE_INTERVALL_SNAKE 300
#define UPDATE_INTERVALL_SNAKE_SLOW 500

#define SNAKE_HEAD_COLOR 0x001F
#define SNAKE_TAIL_COLOR 0xFD40
#define SNAKE_INVISIBLE_COLOR 0x0000
#define SNAKE_IMMORTAL_COLOR 0xFFFF


enum Direction { UP,
                 DOWN,
                 LEFT,
                 RIGHT
               };

typedef struct Segment_ {
  Point p;
  struct Segment_ *next;
} Segment;

class Snake {
  public:
  Segment *head;
    Snake() {}
    Snake(uint8_t playerIndex, Point startPoint);

    void resetSnake();
    void removeSnake();
    bool getIsAlive();
    void reviveSnake();   
    void increaseSize(); 
    void increaseSpeed(); 
    void decreaseSpeed();
    void makeInvisible();
    void makeImmortal();
    void render();
    void setDirection(Direction dir);
    bool isImmortal();
    bool updateSnake(unsigned long currentMillis);
    int isOnSnake(Point p);
    int isOnSnakeOrHead(Point p);
    void removeAfterIndex(int index);

  private:
    Point _startPoint;
    uint8_t _playerIndex;
    uint16_t _snakeHead;
    bool foodEaten = false;
    bool isAlive = true;
    int snakeLength = STARTLENGTH_SNAKE;
    int moveInterval = UPDATE_INTERVALL_SNAKE;
    unsigned long currentMillis = 0;
    unsigned long previousMillis = 0;
    unsigned long velocityMillis = 0;
    unsigned long immortalMillis = 0;
    unsigned long invisibleMillis = 0;
    bool immortal = false;
    bool invisible = false;
    Direction direction = UP;
    void addElement();
    void deleteLastElement();
    Point getNextHeadPosition();
};
#endif