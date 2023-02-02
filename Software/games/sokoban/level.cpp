#include "level.h"
#include <HLM_graphics.h>

#include <cstdio>

#define COLOR_WALL 0x93AA
#define COLOR_BOX 0xD523
#define COLOR_GOAL 0xD0E3
#define COLOR_PLAYER 0x185A
#define COLOR_GOALWITHBOX 0x4609

void sokoban_level::init(const char* level)
{
	for(uint8_t y = 0; y < 32; y++) {
		for(uint8_t x = 0; x < 32; x++) {
			field[x][y] = SOKOBAN_ENTITY_EMPTY;
		}
	}
	won = false;
	width = 0;
	height = 0;
	bool level_has_been_loaded = false;
	for(uint8_t y = 0; y < 32; y++) {
		for(uint8_t x = 0; x < 32; x++) {
			switch(*level) {
				case ' ':
					break;
				case '#':
					field[x][y] = SOKOBAN_ENTITY_WALL;
					break;
				case '$':
					field[x][y] = SOKOBAN_ENTITY_BOX;
					break;
				case '@': 
					player_x = x;
					player_y = y;
					break;
				case '.':
					field[x][y] = SOKOBAN_ENTITY_GOAL;
					break;
				case '+':
					field[x][y] = SOKOBAN_ENTITY_GOAL;
					player_x = x;
					player_y = y;
					break;
				case '*':
					field[x][y] = SOKOBAN_ENTITY_GOALWITHBOX;
					break;
				case '\n':
					if (x > width)
						width = x;
					x = 255; // next for loop iteration will make it 0
					y++;
					break;
				case '\0':
					height = y + 1;
					if (x > width)
						width = x;
					level_has_been_loaded = true;
					x=32; // Exit out of theese loops
					y=32;
					break;
				default:
					break;
			}
			level++;
		}
		if (!level_has_been_loaded)
			width = 32;
	}
	if (!level_has_been_loaded)
		height = 32;
	
	for(uint16_t i = 0; i < 256; i++) {
		last_moves[i] = SOKOBAN_MOVE_NONE;
	}
	
	zoom = 32 / height;
	if (64 / width < zoom)
		zoom = 64 / width;
	
	xoffset = (64 - width * zoom)/2;
	yoffset = (32 - height * zoom)/2;
	
	printf("Loaded: w: %d h: %d z: %d xo: %d yo: %d\n", width, height, zoom, xoffset, yoffset);
}

void sokoban_level::go_left()
{
	if (player_x == 0)
		return;
	sokoban_entity_t target_entity = field[player_x-1][player_y];
	switch (target_entity) {
		case SOKOBAN_ENTITY_EMPTY:
		case SOKOBAN_ENTITY_GOAL:
			player_x--;
			last_moves[++last_move_pointer] = SOKOBAN_MOVE_LEFT;
			break;
		case SOKOBAN_ENTITY_BOX:
		case SOKOBAN_ENTITY_GOALWITHBOX:
			if (player_x == 1)
				return;
			sokoban_entity_t target_entity2 = field[player_x-2][player_y];
			switch (target_entity2) {
				case SOKOBAN_ENTITY_EMPTY:
					field[player_x-2][player_y] = SOKOBAN_ENTITY_BOX;
					field[player_x-1][player_y] = target_entity == SOKOBAN_ENTITY_GOALWITHBOX ? SOKOBAN_ENTITY_GOAL : SOKOBAN_ENTITY_EMPTY;
					player_x--;
					last_moves[++last_move_pointer] = SOKOBAN_MOVE_PUSHLEFT;
					break;
				case SOKOBAN_ENTITY_GOAL:
					field[player_x-2][player_y] = SOKOBAN_ENTITY_GOALWITHBOX;
					field[player_x-1][player_y] = target_entity == SOKOBAN_ENTITY_GOALWITHBOX ? SOKOBAN_ENTITY_GOAL : SOKOBAN_ENTITY_EMPTY;
					player_x--;
					last_moves[++last_move_pointer] = SOKOBAN_MOVE_PUSHLEFT;
					break;
				default: break;
			}
			checkWon();
			break;
	}
}
void sokoban_level::go_right()
{
	if (player_x == 31)
		return;
	sokoban_entity_t target_entity = field[player_x+1][player_y];
	switch (target_entity) {
		case SOKOBAN_ENTITY_EMPTY:
		case SOKOBAN_ENTITY_GOAL:
			player_x++;
			last_moves[++last_move_pointer] = SOKOBAN_MOVE_RIGHT;
			break;
		case SOKOBAN_ENTITY_BOX:
		case SOKOBAN_ENTITY_GOALWITHBOX:
			if (player_x == 30)
				return;
			sokoban_entity_t target_entity2 = field[player_x+2][player_y];
			switch (target_entity2) {
				case SOKOBAN_ENTITY_EMPTY:
					field[player_x+2][player_y] = SOKOBAN_ENTITY_BOX;
					field[player_x+1][player_y] = target_entity == SOKOBAN_ENTITY_GOALWITHBOX ? SOKOBAN_ENTITY_GOAL : SOKOBAN_ENTITY_EMPTY;
					player_x++;
					last_moves[++last_move_pointer] = SOKOBAN_MOVE_PUSHRIGHT;
					break;
				case SOKOBAN_ENTITY_GOAL:
					field[player_x+2][player_y] = SOKOBAN_ENTITY_GOALWITHBOX;
					field[player_x+1][player_y] = target_entity == SOKOBAN_ENTITY_GOALWITHBOX ? SOKOBAN_ENTITY_GOAL : SOKOBAN_ENTITY_EMPTY;
					player_x++;
					last_moves[++last_move_pointer] = SOKOBAN_MOVE_PUSHRIGHT;
					break;
				default: break;
			}
			checkWon();
			break;
	}
}
void sokoban_level::go_up()
{
	if (player_y == 0)
		return;
	sokoban_entity_t target_entity = field[player_x][player_y-1];
	switch (target_entity) {
		case SOKOBAN_ENTITY_EMPTY:
		case SOKOBAN_ENTITY_GOAL:
			player_y--;
			last_moves[++last_move_pointer] = SOKOBAN_MOVE_UP;
			break;
		case SOKOBAN_ENTITY_BOX:
		case SOKOBAN_ENTITY_GOALWITHBOX:
			if (player_y == 1)
				return;
			sokoban_entity_t target_entity2 = field[player_x][player_y-2];
			switch (target_entity2) {
				case SOKOBAN_ENTITY_EMPTY:
					field[player_x][player_y-2] = SOKOBAN_ENTITY_BOX;
					field[player_x][player_y-1] = target_entity == SOKOBAN_ENTITY_GOALWITHBOX ? SOKOBAN_ENTITY_GOAL : SOKOBAN_ENTITY_EMPTY;
					player_y--;
					last_moves[++last_move_pointer] = SOKOBAN_MOVE_PUSHUP;
					break;
				case SOKOBAN_ENTITY_GOAL:
					field[player_x][player_y-2] = SOKOBAN_ENTITY_GOALWITHBOX;
					field[player_x][player_y-1] = target_entity == SOKOBAN_ENTITY_GOALWITHBOX ? SOKOBAN_ENTITY_GOAL : SOKOBAN_ENTITY_EMPTY;
					player_y--;
					last_moves[++last_move_pointer] = SOKOBAN_MOVE_PUSHUP;
					break;
				default: break;
			}
			checkWon();
			break;
	}
}
void sokoban_level::go_down()
{
	if (player_y == 31)
		return;
	sokoban_entity_t target_entity = field[player_x][player_y+1];
	switch (target_entity) {
		case SOKOBAN_ENTITY_EMPTY:
		case SOKOBAN_ENTITY_GOAL:
			player_y++;
			last_moves[++last_move_pointer] = SOKOBAN_MOVE_DOWN;
			break;
		case SOKOBAN_ENTITY_BOX:
		case SOKOBAN_ENTITY_GOALWITHBOX:
			if (player_y == 30)
				return;
			sokoban_entity_t target_entity2 = field[player_x][player_y+2];
			switch (target_entity2) {
				case SOKOBAN_ENTITY_EMPTY:
					field[player_x][player_y+2] = SOKOBAN_ENTITY_BOX;
					field[player_x][player_y+1] = target_entity == SOKOBAN_ENTITY_GOALWITHBOX ? SOKOBAN_ENTITY_GOAL : SOKOBAN_ENTITY_EMPTY;
					player_y++;
					last_moves[++last_move_pointer] = SOKOBAN_MOVE_PUSHDOWN;
					break;
				case SOKOBAN_ENTITY_GOAL:
					field[player_x][player_y+2] = SOKOBAN_ENTITY_GOALWITHBOX;
					field[player_x][player_y+1] = target_entity == SOKOBAN_ENTITY_GOALWITHBOX ? SOKOBAN_ENTITY_GOAL : SOKOBAN_ENTITY_EMPTY;
					player_y++;
					last_moves[++last_move_pointer] = SOKOBAN_MOVE_PUSHDOWN;
					break;
				default: break;
			}
			checkWon();
			break;
	}
}

void sokoban_level::draw()
{
	for(uint8_t x = 0; x < 32; x++) {
		for(uint8_t y = 0; y < 32; y++) {
			switch(field[x][y]) {
				case SOKOBAN_ENTITY_WALL: drawField(x,y,COLOR_WALL); break;
				case SOKOBAN_ENTITY_BOX: drawField(x,y,COLOR_BOX); break;
				case SOKOBAN_ENTITY_GOAL: drawField(x,y,COLOR_GOAL); break;
				case SOKOBAN_ENTITY_GOALWITHBOX: drawField(x,y,COLOR_GOALWITHBOX); break;
				default: break;
			}
		}
	}
	drawField(player_x, player_y, COLOR_PLAYER);
}

void sokoban_level::drawField(uint8_t x, uint8_t y, uint16_t color)
{
	if (zoom <= 1) {
		get_graphics()->drawPixel(xoffset + x, yoffset + y , color);
	} else {
		get_graphics()->fillRect(xoffset + x * zoom, yoffset + y * zoom, zoom, zoom, color);
	}
}


void sokoban_level::undo()
{
	switch(last_moves[last_move_pointer]) {
		case SOKOBAN_MOVE_NONE: return;
		case SOKOBAN_MOVE_LEFT: player_x++; break;
		case SOKOBAN_MOVE_RIGHT: player_x--; break;
		case SOKOBAN_MOVE_UP: player_y++; break;
		case SOKOBAN_MOVE_DOWN: player_y--; break;
		case SOKOBAN_MOVE_PUSHLEFT:
			switch (field[player_x - 1][player_y]) {
				case SOKOBAN_ENTITY_BOX: field[player_x - 1][player_y] = SOKOBAN_ENTITY_EMPTY; break;
				case SOKOBAN_ENTITY_GOALWITHBOX: field[player_x - 1][player_y] = SOKOBAN_ENTITY_GOAL; break;
			}
			switch (field[player_x][player_y]) {
				case SOKOBAN_ENTITY_EMPTY: field[player_x][player_y] = SOKOBAN_ENTITY_BOX; break;
				case SOKOBAN_ENTITY_GOAL: field[player_x][player_y] = SOKOBAN_ENTITY_GOALWITHBOX; break;
			}
			player_x++; 
			break;
		case SOKOBAN_MOVE_PUSHRIGHT:
			switch (field[player_x + 1][player_y]) {
				case SOKOBAN_ENTITY_BOX: field[player_x + 1][player_y] = SOKOBAN_ENTITY_EMPTY; break;
				case SOKOBAN_ENTITY_GOALWITHBOX: field[player_x + 1][player_y] = SOKOBAN_ENTITY_GOAL; break;
			}
			switch (field[player_x][player_y]) {
				case SOKOBAN_ENTITY_EMPTY: field[player_x][player_y] = SOKOBAN_ENTITY_BOX; break;
				case SOKOBAN_ENTITY_GOAL: field[player_x][player_y] = SOKOBAN_ENTITY_GOALWITHBOX; break;
			}
			player_x--;
			break;
		case SOKOBAN_MOVE_PUSHUP:
			switch (field[player_x][player_y - 1]) {
				case SOKOBAN_ENTITY_BOX: field[player_x][player_y - 1] = SOKOBAN_ENTITY_EMPTY; break;
				case SOKOBAN_ENTITY_GOALWITHBOX: field[player_x][player_y - 1] = SOKOBAN_ENTITY_GOAL; break;
			}
			switch (field[player_x][player_y]) {
				case SOKOBAN_ENTITY_EMPTY: field[player_x][player_y] = SOKOBAN_ENTITY_BOX; break;
				case SOKOBAN_ENTITY_GOAL: field[player_x][player_y] = SOKOBAN_ENTITY_GOALWITHBOX; break;
			}
			player_y++;
			break;
		case SOKOBAN_MOVE_PUSHDOWN:
			switch (field[player_x][player_y + 1]) {
				case SOKOBAN_ENTITY_BOX: field[player_x][player_y + 1] = SOKOBAN_ENTITY_EMPTY; break;
				case SOKOBAN_ENTITY_GOALWITHBOX: field[player_x][player_y + 1] = SOKOBAN_ENTITY_GOAL; break;
			}
			switch (field[player_x][player_y]) {
				case SOKOBAN_ENTITY_EMPTY: field[player_x][player_y] = SOKOBAN_ENTITY_BOX; break;
				case SOKOBAN_ENTITY_GOAL: field[player_x][player_y] = SOKOBAN_ENTITY_GOALWITHBOX; break;
			}
			player_y--;
			break;
	}
	last_moves[last_move_pointer--] = SOKOBAN_MOVE_NONE;
}

bool sokoban_level::isWon()
{
	return won;
}

void sokoban_level::checkWon()
{
	won = true;
	for(uint8_t y = 0; y < 32; y++) {
		for(uint8_t x = 0; x < 32; x++) {
			if (field[x][y] == SOKOBAN_ENTITY_BOX) {
				won = false;
				return;
			}
		}
	}
}


void sokoban_level::free()
{
}
