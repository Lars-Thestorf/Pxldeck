#include "tetris_game.h"
#include <HLM_random.h>
#include <HLM_time.h>
#include <stdio.h>

//All values in Frames
#define DAS_DELAY     12 //Delayed Auto Shift initial Delay
#define DAS_REPEAT     5 //Delayed Auto Shift repeating Delay
#define ARE_DELAY      2 //Apearance Delay
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
uint8_t empty_tetronimo[4][4] = {
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
};

static const uint8_t ticksPerLevel[21] = {53,49,45,41,37,33,28,22,17,11,10,9,8,7,6,6,5,5,4,4,3};

#define INGAME_STATE_ARE       0 //wait until next tetronimo apears
#define INGAME_STATE_DROP      1 //tetronimo dropping
#define INGAME_STATE_LINECLEAR 2 //line clear flashing animation
#define INGAME_STATE_OVER      3 //Game Over fill animation

void tetris_game::init()
{
	game_state = GAME_STATE_MENU;
	ingame_state = INGAME_STATE_ARE;
	are_counter = 0;
	score = 0;
	lines = 0;
	level = 9;
	for (uint8_t line = 0; line < FIELD_HEIGHT; line++) {
		for (uint8_t x=0; x < FIELD_WIDTH; x++) {
			field[x][line] = 0;
		}
	}
	currentx = 3;
	currenty = -3;
	currentBlockId = getRandomNumber(7);
	nextBlockId = getRandomNumber(7);
	rotation = 0;
	currentblock = &empty_tetronimo;
	nextblock = &(tetronimos[nextBlockId][rotation]);
}
void tetris_game::tick()
{
	if (game_state == GAME_STATE_PLAY && !game_paused) {
		if (getSystemTime() > lastFrameTime + 16) {
			lastFrameTime = getSystemTime();
			switch(ingame_state) {
				case INGAME_STATE_ARE:
					are_counter++;
					if (are_counter > ARE_DELAY) {
						are_counter = 0;
						currentBlockId = nextBlockId;
						currentblock = nextblock;
						nextBlockId = getRandomNumber(7);
						rotation = 0;
						nextblock = &(tetronimos[nextBlockId][rotation]);
						currentx = 3;
						currenty = -3;
						framesToDrop = ticksPerLevel[level > 20 ? 20 : level];
						ingame_state = INGAME_STATE_DROP;
					}
					break;
				case INGAME_STATE_DROP:
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
						framesToDrop = ticksPerLevel[level > 20 ? 20 : level];
						if (holdingDownFrames > HOLDDROP_DELAY)
							score += level+1;
						holdingDownFrames = 0;
						if (doesTetronimoFit(currentblock, currentx, currenty + 1)) {
							currenty++;
						} else {
							if (placeTetronimo(currentblock, currentx, currenty)){
								currentblock = &empty_tetronimo;
								for (uint8_t line = 0; line < FIELD_HEIGHT; line++) {
									if (checkLineFull(line)) {
										ingame_state = INGAME_STATE_LINECLEAR;
										flashAnimation = 90;
									}
								}
								if (ingame_state != INGAME_STATE_LINECLEAR)
									ingame_state = INGAME_STATE_ARE;
							} else { // game over
								currentblock = &empty_tetronimo;
								ingame_state = INGAME_STATE_OVER;
								flashAnimation = 80;
							}
						} 
					}
					break;
				case INGAME_STATE_LINECLEAR:
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
										field[x][dropline] = field[x][dropline-1];
									}
								}
								for (uint8_t x=0; x < FIELD_WIDTH; x++) {
									field[x][0] = 0;
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
						if (level < lines/10)
							level++;
						ingame_state = INGAME_STATE_ARE;
					}
					break;
				case INGAME_STATE_OVER:
					if (flashAnimation%5 == 0) {
						for (uint8_t x = 0; x < FIELD_WIDTH; x++) {
							field[x][(flashAnimation/5) - 1] = 0x80;
						}
					}
					flashAnimation--;
					if (!flashAnimation) {
						game_state = GAME_STATE_OVER;
						game_end(this);
					}
					break;
			}
		}
	}
}
void tetris_game::left(bool pressed)
{
	if (game_state == GAME_STATE_MENU && pressed){
		if (level > 0)
			level--;
	}
}
void tetris_game::right(bool pressed)
{
	if (game_state == GAME_STATE_MENU && pressed){
			if (level < 9)
				level++;
	}
}

void tetris_game::rotateR(bool pressed)
{
	if ((game_state == GAME_STATE_MENU || game_state == GAME_STATE_OVER) && pressed){
		pause(true);
	}
	if (game_state == GAME_STATE_PLAY && !game_paused && pressed) {
		rotate(rotation + 1);
	}
}
void tetris_game::rotateL(bool pressed)
{
	if (game_state == GAME_STATE_PLAY && !game_paused && pressed) {
		rotate(rotation - 1);
	}
}
void tetris_game::rotate(uint8_t dir)
{
	if (doesTetronimoFit(&tetronimos[currentBlockId][dir%4], currentx, currenty)) {
		rotation = dir%4;
		currentblock = &(tetronimos[currentBlockId][rotation]);
	}
}


void tetris_game::pause(bool pressed)
{
	if (pressed) {
		switch (game_state) {
			case GAME_STATE_PLAY:
				game_paused=!game_paused;
				break;
			case GAME_STATE_OVER:
				{
					uint8_t prev_level = level;
					init();
					level = prev_level > 9 ? 9 : prev_level;
					game_state = GAME_STATE_MENU;
				}
				break;
			case GAME_STATE_MENU:
				game_state = GAME_STATE_PLAY;
		}
	}
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
	return HLM_random(max);
}
uint64_t tetris_game::getSystemTime()
{
	return get_ms_since_boot();
}


void tetris_game::free()
{
	
}
