#include "tetris_game.h"
#include <esp_system.h>
#include <esp_timer.h>

//All values in Frames
#define DAS_DELAY     16 //Delayed Auto Shift initial Delay
#define DAS_REPEAT     6 //Delayed Auto Shift repeating Delay
#define ARE            2 //Apearance Delay (unimplemented)
#define HOLDDROP_DELAY 3 //Softdrop delay

static const uint8_t tetronimos[7][4][4][4] = {
	{
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
		},
		{
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0},
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
		},
		{
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0},
			{0,0,1,0},
		},
	},
	{
		{
			{0,0,0,0},
			{2,2,0,0},
			{0,2,2,0},
			{0,0,0,0},
		},
		{
			{0,2,0,0},
			{2,2,0,0},
			{2,0,0,0},
			{0,0,0,0},
		},
		{
			{0,0,0,0},
			{2,2,0,0},
			{0,2,2,0},
			{0,0,0,0},
		},
		{
			{0,2,0,0},
			{2,2,0,0},
			{2,0,0,0},
			{0,0,0,0},
		},
	},
	{
		{
			{0,0,0,0},
			{0,3,3,0},
			{3,3,0,0},
			{0,0,0,0},
		},
		{
			{0,3,0,0},
			{0,3,3,0},
			{0,0,3,0},
			{0,0,0,0},
		},
		{
			{0,0,0,0},
			{0,3,3,0},
			{3,3,0,0},
			{0,0,0,0},
		},
		{
			{0,3,0,0},
			{0,3,3,0},
			{0,0,3,0},
			{0,0,0,0},
		},
	},
	{
		{
			{0,0,0,0},
			{0,4,0,0},
			{4,4,4,0},
			{0,0,0,0},
		},
		{
			{0,0,0,0},
			{0,4,0,0},
			{0,4,4,0},
			{0,4,0,0},
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{4,4,4,0},
			{0,4,0,0},
		},
		{
			{0,0,0,0},
			{0,4,0,0},
			{4,4,0,0},
			{0,4,0,0},
		},
	},
	{
		{
			{0,0,0,0},
			{5,0,0,0},
			{5,5,5,0},
			{0,0,0,0},
		},
		{
			{0,0,0,0},
			{0,5,5,0},
			{0,5,0,0},
			{0,5,0,0},
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{5,5,5,0},
			{0,0,5,0},
		},
		{
			{0,0,0,0},
			{0,5,0,0},
			{0,5,0,0},
			{5,5,0,0},
		},
	},
	{
		{
			{0,0,0,0},
			{0,0,6,0},
			{6,6,6,0},
			{0,0,0,0},
		},
		{
			{0,0,0,0},
			{0,6,0,0},
			{0,6,0,0},
			{0,6,6,0},
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{6,6,6,0},
			{6,0,0,0},
		},
		{
			{0,0,0,0},
			{6,6,0,0},
			{0,6,0,0},
			{0,6,0,0},
		},
	},
	{
		{
			{0,0,0,0},
			{0,7,7,0},
			{0,7,7,0},
			{0,0,0,0},
		},
		{
			{0,0,0,0},
			{0,7,7,0},
			{0,7,7,0},
			{0,0,0,0},
		},
		{
			{0,0,0,0},
			{0,7,7,0},
			{0,7,7,0},
			{0,0,0,0},
		},
		{
			{0,0,0,0},
			{0,7,7,0},
			{0,7,7,0},
			{0,0,0,0},
		},
	},
};
static const uint8_t ticksPerLevel[21] = {53,49,45,41,37,33,28,22,17,11,10,9,8,7,6,6,5,5,4,4,3};

void tetris_game::init()
{
	score = 0;
	lines = 0;
	level = 9;
	currentx = 3;
	currenty = -3;
	currentBlockId = getRandomNumber(7);
	nextBlockId = getRandomNumber(7);
	rotation = 0;
	currentblock = &(tetronimos[currentBlockId][rotation]);
	nextblock = &(tetronimos[nextBlockId][rotation]);
	flashAnimation = 0;
	framesToDrop = 1;
}
void tetris_game::tick()
{
	if (!game_paused) {
		if (getSystemTime() > lastFrameTime + 16 * 1000) {
			lastFrameTime = getSystemTime();
			if (holdingDown)
				holdingDownFrames++;
			else
				holdingDownFrames = 0;
			if (holdingLeft)
				holdingLeftFrames++;
			else
				holdingLeftFrames = 0;
			if (holdingRight)
				holdingRightFrames++;
			else
				holdingRightFrames = 0;
			if (!flashAnimation) {
				if (holdingLeftFrames == 1 || holdingLeftFrames > DAS_DELAY){
					if (doesTetronimoFit(currentblock, currentx - 1, currenty)) {
						currentx--;
					}
					if (holdingLeftFrames > DAS_DELAY) {
						holdingLeftFrames = DAS_DELAY-DAS_REPEAT;
					}
				}
				if (holdingRightFrames == 1 || holdingRightFrames > DAS_DELAY) {
					if (doesTetronimoFit(currentblock, currentx + 1, currenty)) {
						currentx++;
					}
					if (holdingRightFrames > DAS_DELAY) {
						holdingRightFrames = DAS_DELAY-DAS_REPEAT;
					}
				}
				framesToDrop--;
				if (!framesToDrop || holdingDownFrames > HOLDDROP_DELAY) {
					if (holdingDownFrames > HOLDDROP_DELAY)
						score += level+1;
					holdingDownFrames = 0;
					framesToDrop = ticksPerLevel[level > 20 ? 20 : level];
					if (doesTetronimoFit(currentblock, currentx, currenty + 1)) {
						currenty++;
					} else {
						if (placeTetronimo(currentblock, currentx, currenty)){
							currentBlockId = nextBlockId;
							currentblock = nextblock;
							nextBlockId = getRandomNumber(7);
							rotation = 0;
							nextblock = &(tetronimos[nextBlockId][rotation]);
							currentx = 3;
							currenty = -3;
							for (uint8_t line = 0; line < FIELD_HEIGHT; line++) {
								if (checkLineFull(line)) {
									flashAnimation = 90;
								}
							}
						} else { // game over
							
						}
					} 
				}
			} else {
				if (flashAnimation%15 == 0) {
					for (uint8_t line = 0; line < FIELD_HEIGHT; line++) {
						if (checkLineFull(line)) {
							for (uint8_t x=0; x < FIELD_WIDTH; x++) {
								field[x][line] ^= 0xFF;
							}
						}
					}
				}
				flashAnimation--;
				if(!flashAnimation) {
					uint8_t linecount = 0;
					for (uint8_t line = 0; line < FIELD_HEIGHT; line++) {
						if (checkLineFull(line)) {
							for (uint8_t dropline = line; dropline > 0; dropline--) {
								for (uint8_t x=0; x < FIELD_WIDTH; x++) {
									if (dropline > 0)
										field[x][dropline] = field[x][dropline-1];
									else
										field[x][dropline] = 0;
								}
							}
							linecount++;
						}
					}
					lines+=linecount;
					switch(linecount){
						case 1:
							score += (level+1)*40;
							break;
						case 2:
							score += (level+1)*100;
							break;
						case 3:
							score += (level+1)*300;
							break;
						case 4:
							score += (level+1)*1200;
							break;
					}
					if (level <= lines/10)
						level++;
				}
			}
		}
	}
}
void tetris_game::left(bool pressed)
{
	holdingLeft = pressed;
}
void tetris_game::right(bool pressed)
{
	holdingRight = pressed;
}
void tetris_game::rotateR(bool pressed)
{
	if (pressed && !holdingRotateRPrev)
		rotate(rotation + 1);
	holdingRotateRPrev = pressed;
}
void tetris_game::rotateL(bool pressed)
{
	if (pressed && !holdingRotateLPrev)
		rotate(rotation - 1);
	holdingRotateLPrev = pressed;
}
void tetris_game::rotate(uint8_t dir)
{
	if (doesTetronimoFit(&tetronimos[currentBlockId][dir%4], currentx, currenty)) {
		rotation = dir%4;
		currentblock = &(tetronimos[currentBlockId][rotation]);
	}
}
void tetris_game::down(bool pressed)
{
	holdingDown = pressed;
}
void tetris_game::pause(bool pressed)
{
	if (pressed && !holdingPausePrev)
		game_paused=!game_paused;
	holdingPausePrev = pressed;
}



bool tetris_game::doesTetronimoFit(const uint8_t (*tetronimo)[4][4], int8_t x, int8_t y)
{
	for (uint8_t _y = 0; _y < 4; _y++){
		for (uint8_t _x = 0; _x < 4; _x++){
			if ((*tetronimo)[_y][_x] != 0) {
				//if (x+_x < 0 || y+_y < 0)
				if (x+_x < 0)
					return false;
				if (x+_x >= FIELD_WIDTH || y+_y >= FIELD_HEIGHT)
					return false;
				if (y+_y >= 0)
					if (field[x+_x][y+_y] != 0)
						return false;
			}
		}
	}
	return true;
}
bool tetris_game::placeTetronimo(const uint8_t (*tetronimo)[4][4] ,int8_t x, int8_t y)
{
	for (uint8_t _y = 0; _y < 4; _y++){
		for (uint8_t _x = 0; _x < 4; _x++){
			if ((*tetronimo)[_y][_x] != 0) {
				if (y+_y < 0)
					return false;
				field[x+_x][y+_y] = (*tetronimo)[_y][_x];
			}
		}
	}
	return true;
}
bool tetris_game::checkLineFull(uint8_t line)
{
	for (uint8_t x=0; x < FIELD_WIDTH; x++){
		if (!field[x][line]){
			return false;
		}
	}
	return true;
}

uint16_t tetris_game::getRandomNumber(uint16_t max)
{
	return esp_random() % max;
}
uint64_t tetris_game::getSystemTime()
{
	return esp_timer_get_time();
}

void tetris_game::free()
{
	
}
