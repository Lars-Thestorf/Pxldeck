#ifndef _LALEMA_GAME_SOKOBAN_LEVEL_H_
#define _LALEMA_GAME_SOKOBAN_LEVEL_H_

#include "stdint.h"

#define sokoban_entity_t uint8_t
#define SOKOBAN_ENTITY_EMPTY 0
#define SOKOBAN_ENTITY_WALL 1
#define SOKOBAN_ENTITY_BOX 2
#define SOKOBAN_ENTITY_GOAL 3
#define SOKOBAN_ENTITY_GOALWITHBOX 4
#define sokoban_move_t uint8_t
#define SOKOBAN_MOVE_NONE 0
#define SOKOBAN_MOVE_UP 1
#define SOKOBAN_MOVE_DOWN 2
#define SOKOBAN_MOVE_LEFT 3
#define SOKOBAN_MOVE_RIGHT 4
#define SOKOBAN_MOVE_PUSHUP 5
#define SOKOBAN_MOVE_PUSHDOWN 6
#define SOKOBAN_MOVE_PUSHLEFT 7
#define SOKOBAN_MOVE_PUSHRIGHT 8

class sokoban_level {
    public:
		void init(const char* level);
		void free();
		void go_left();
		void go_right();
		void go_up();
		void go_down();
		void undo();
		void draw();
		bool isWon();
    private:
		sokoban_entity_t field[32][32]; //1kb
		sokoban_move_t last_moves[256];
		uint8_t last_move_pointer;
		uint16_t moves;
		uint8_t player_x;
		uint8_t player_y;
		uint8_t height;
		uint8_t width;
		uint8_t zoom;
		uint8_t xoffset;
		uint8_t yoffset;
		void drawField(uint8_t x, uint8_t y, uint16_t color);
		bool won;
		void checkWon();
};

#endif

