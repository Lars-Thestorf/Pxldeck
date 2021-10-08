#ifndef _LALEMA_GAME_TETRISGAME_H_
#define _LALEMA_GAME_TETRISGAME_H_

#define FIELD_HEIGHT 16
#define FIELD_WIDTH 10

#include <stdint.h>
#include <stdbool.h>
	
class tetris_game
    {
    public:
		void init();
		void free();
        void tick();
		void left(bool pressed);
		void right(bool pressed);
		void down(bool pressed);
		void pause(bool pressed);
		void rotateL(bool pressed);
		void rotateR(bool pressed);
		void rotate(uint8_t dir);
		uint8_t field[FIELD_WIDTH][FIELD_HEIGHT];
		int8_t currentx;
		int8_t currenty;
		const uint8_t (*currentblock)[4][4];
		const uint8_t (*nextblock)[4][4];
		uint8_t level;
		uint32_t score;
		uint16_t lines;
    private:
		bool game_paused;
		uint16_t flashAnimation;
		uint64_t lastFrameTime;
		uint8_t rotation;
		uint8_t currentBlockId;
		uint8_t nextBlockId;
		uint8_t framesToDrop;
		bool holdingDown;
		bool holdingLeft;
		bool holdingRight;
		bool holdingRotateRPrev;
		bool holdingRotateLPrev;
		bool holdingPausePrev;
		uint8_t holdingDownFrames;
		uint8_t holdingLeftFrames;
		uint8_t holdingRightFrames;
		bool placeTetronimo(const uint8_t (*tetronimo)[4][4], int8_t x, int8_t y);
		bool checkLineFull(uint8_t line);
		bool doesTetronimoFit(const uint8_t (*tetronimo)[4][4], int8_t x, int8_t y);
        uint16_t getRandomNumber(uint16_t max);
		uint64_t getSystemTime();
    };

#endif
