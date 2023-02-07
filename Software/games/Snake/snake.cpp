#include "snake.h"
#include <stdlib.h>

Snake::Snake(uint8_t playerIndex,  Point startPoint) {
    _startPoint = startPoint;
    _playerIndex = playerIndex;
    _snakeHead = SNAKE_HEAD_COLOR;
    head = (Segment *)malloc(sizeof(Segment));
    resetSnake();
}

void Snake::resetSnake() {
    head->p = _startPoint;
    head->next = NULL;
    setDirection(UP);
    for (int i = 0; i < STARTLENGTH_SNAKE; i++) {
    addElement();
    }
}

void Snake::removeSnake() {
    removeAfterIndex(2);
    isAlive = false;
}

bool Snake::getIsAlive(){
    return isAlive;
}

void Snake::reviveSnake() {
    isAlive = true;
}
    
void Snake::increaseSize() {
    foodEaten = true;
}

void Snake::increaseSpeed() {
    moveInterval = UPDATE_INTERVALL_SNAKE_SPEED;
    velocityMillis = currentMillis;
}

void Snake::decreaseSpeed() {
    moveInterval = UPDATE_INTERVALL_SNAKE_SLOW;
    velocityMillis = currentMillis;
}

void Snake::makeInvisible(){
    invisible = true;
    invisibleMillis = currentMillis;
}
void Snake::makeImmortal() {
    immortal = true;
    immortalMillis = currentMillis;
}

void Snake::render() {
    HLM_graphics *gfx = get_graphics();
    if (isAlive) {
        gfx->drawPixel(head->p.x,head->p.y,_snakeHead);
        Segment *segment = head->next;
        int snakePos = 0;
        while (segment != NULL) {
            if(invisible){
                gfx->drawPixel(segment->p.x,segment->p.y,SNAKE_INVISIBLE_COLOR);
            }else if(immortal){
                gfx->drawPixel(segment->p.x,segment->p.y,SNAKE_IMMORTAL_COLOR);
            }else{
                gfx->drawPixel(segment->p.x,segment->p.y,SNAKE_TAIL_COLOR);
            }
            snakePos++;
            segment = segment->next;
        }
    }
}

void Snake::setDirection(Direction dir) {
    direction = dir;
}

bool Snake::isImmortal() {
    return immortal;
}

bool Snake::updateSnake(unsigned long currentMillis) {
    this->currentMillis = currentMillis;
    if (currentMillis - velocityMillis >= EFFECT_DURATION) {
        moveInterval = UPDATE_INTERVALL_SNAKE;
    }
    if (currentMillis - immortalMillis >= EFFECT_DURATION) {
        immortal = false;
    }
    if (currentMillis - invisibleMillis >= EFFECT_DURATION) {
        invisible = false;
    }
    if (currentMillis - previousMillis >= moveInterval) {
        previousMillis = currentMillis;
        if (isAlive) {
            addElement();
            if (foodEaten) {
                foodEaten = false;
                snakeLength++;
            } else {
                deleteLastElement();
            }
        }
        return true;
    }
    return false;
}

int Snake::isOnSnake(Point p) {
    if (isAlive) {
        int i = 2;
        Segment *segment = head->next;
        while (segment != NULL) {
            if (arePointsEqual(segment->p, p)) {
                return i;
            }
            i++;
            segment = segment->next;
        }
    }
    return 0;
}

int Snake::isOnSnakeOrHead(Point p) {
    if (!isAlive) {
        return 0;
    }
    if (arePointsEqual(head->p, p)) {
        return 1;
    }
    return isOnSnake(p);
}

void Snake::removeAfterIndex(int index) {
    if (immortal || index == 0) {
        return;
    }
    if (index == 1) {
        removeSnake();
    }
    Segment *segment = head->next;
    Segment *segmentLast;
    for (int i = 2; i < index; i++) {
        segment = segment->next;
    }
    segmentLast = segment;
    segment = segment->next;
    segmentLast->next = NULL;
    while (segment != NULL) {
        segmentLast = segment;
        segment = segment->next;
        free(segmentLast);
    }
}

void Snake::addElement() {
    Segment *newElement = (Segment *)malloc(sizeof(Segment));
    newElement->next = head;
    newElement->p = getNextHeadPosition();
    head = newElement;
}

void Snake::deleteLastElement() {
    Segment *p = head, *previous = NULL;
    while (p->next != NULL) {
        previous = p;
        p = p->next;
    }
    previous->next = NULL;
    free(p);
}

Point Snake::getNextHeadPosition() {
    Point nextHead = head->p;
    switch (direction) {
        case UP:
            if (++nextHead.y >= 32) {
                nextHead.y = 0;
            }
            break;
        case DOWN:
            if (--nextHead.y < 0) {
                nextHead.y += 32;
            }
            break;
        case RIGHT:
            if (++nextHead.x >= 64) {
                nextHead.x = 0;
            }
            break;
        case LEFT:
            if (--nextHead.x < 0) {
                nextHead.x += 64;
            }
            break;
    }
    return nextHead;
}
