#include "tictactoe.h"

#include <HLM_network.h>
#include <HLM_graphics.h>
#include <defaultInputEvents.h>

#include <stdio.h>
#include <cstdlib>

const uint8_t icon[] = {0x00, 0x01, 0x20, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00};
const HLM_game tictactoe_game = {
	"TicTacToe",
	icon,
	*tictactoe_setup,
	*tictactoe_loop,
	*tictactoe_free
};

typedef enum tictactoe_field_e {
    TICTACTOE_NONE,
    TICTACTOE_CIRCLE,
    TICTACTOE_CROSS,
} tictactoe_field_e;

typedef enum tictactoe_states_e{
    TICTACTOE_WAITING,
    TICTACTOE_INGAME,
    TICTACTOE_GAME_END
}ttictactoe_states_e;

typedef struct tictactoe_mem_t {
	tictactoe_states_e state;
    bool isCross;
    bool gotFirst;
    bool yourTurn;
    uint8_t cursor_x;
    uint8_t cursor_y;
    tictactoe_field_e board[3][3];
    device_id_t otherPlayer;
} tictactoe_mem_t;

#define TICMEM ((tictactoe_mem_t*)gamemem)

#define MSG_START 74
#define MSG_START2 70
#define MSG_PLACE 98
#define MSG_CLOSE 48

tictactoe_mem_t *ticmem;

bool check_if_full () {
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(ticmem->board[i][j] == TICTACTOE_NONE){
                return false;
            }
        }
    }
    return true;
}
bool if_winner(tictactoe_field_e player){
    for(int i = 0;i < 3;i++){
        if(ticmem->board[i][0] == player && ticmem->board[i][1] == player && ticmem->board[i][2] == player){
            return true;
        }
        if(ticmem->board[0][i] == player && ticmem->board[1][i] == player && ticmem->board[2][i] == player){
            return true;
        }
    }
    if(ticmem->board[0][0] == player && ticmem->board[1][1] == player && ticmem->board[2][2] == player){
        return true;
    }
    if(ticmem->board[2][0] == player && ticmem->board[1][1] == player && ticmem->board[0][2] == player){
        return true;
    }
    return false;
}
void isGameOver(tictactoe_field_e field) {
    if (if_winner(field)) {
        ticmem->state = TICTACTOE_GAME_END;
    }
    if (check_if_full()) {
        ticmem->state = TICTACTOE_GAME_END;
    }
}
void network_callback(uint8_t device, const uint8_t *msg, size_t len) {
	printf("Habe %d bytes empfangen von %d\n", len, device);
    if (!ticmem->gotFirst) {
        ticmem->gotFirst = true;
        return;
    }
    if (msg[0] == MSG_START) {
        printf("S1\n");
        ticmem->otherPlayer = device;
        ticmem->state = TICTACTOE_INGAME;
        ticmem->yourTurn = true;
        ticmem->isCross = false;
        uint8_t startmsg = MSG_START2;
        network_unicast(device,&startmsg,1);
        return;
    }
    if (msg[0] == MSG_START2) {
        printf("S2\n");
        ticmem->otherPlayer = device;
        ticmem->state = TICTACTOE_INGAME;
        ticmem->yourTurn = false;
        ticmem->isCross = true;
    }
    if (msg[0] == MSG_PLACE) {
        printf("P\n");
        ticmem->board[msg[1]][msg[2]] = ticmem->isCross ? TICTACTOE_CIRCLE : TICTACTOE_CROSS;
        isGameOver(ticmem->isCross ? TICTACTOE_CROSS : TICTACTOE_CIRCLE);
        ticmem->yourTurn = true;
    }
    if (msg[0] == MSG_CLOSE) {
        ticmem->state = TICTACTOE_WAITING;
        for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            ticmem->board[i][j] = TICTACTOE_NONE;
        }
    }
    }
}
void draw_cross(uint8_t x, uint8_t y){
    get_graphics()->drawText(18 + x * 11, y * 11, "x", 0xff00);
}
void draw_circle(uint8_t x, uint8_t y){
    get_graphics()->drawText(18 + x * 11, y * 11, "o", 0x00ff);
}
void draw_cursor(uint8_t x, uint8_t y, bool turn) {
    get_graphics()->drawRect(16+x*11,y*11,10,10,turn ? 0x07E0 : 0x8965);
}
void draw_field(){
    get_graphics()->drawHLine(16,10,32,0xffff);
    get_graphics()->drawHLine(16,21,32,0xffff);
    get_graphics()->drawVLine(26,0,32,0xffff);
    get_graphics()->drawVLine(37,0,32,0xffff);
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            switch(ticmem->board[i][j]){
                case TICTACTOE_CIRCLE:
                    draw_circle(i,j);
                    break;
                case TICTACTOE_CROSS:
                    draw_cross(i,j);
                    break;
                case TICTACTOE_NONE:
                    break;
            }
        }
    }
}

void* tictactoe_setup() {
	void* gamemem = malloc(sizeof(tictactoe_mem_t));
    ticmem = TICMEM;
    TICMEM->state = TICTACTOE_WAITING;
    ticmem->isCross = false;
    ticmem->gotFirst = false;
    ticmem->cursor_x = 1;
    ticmem->cursor_y = 1;
    network_init(network_callback);
    uint8_t startmsg = MSG_START;
    network_broadcast(&startmsg, 1);

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            TICMEM->board[i][j] = TICTACTOE_NONE;
        }
    }
	return gamemem;
}
void tictactoe_loop(void* gamemem) {
    switch (ticmem->state){
        case TICTACTOE_WAITING:
            get_graphics()->drawText(0,0,"Waiting",0xFFFF);
            break;
        case TICTACTOE_INGAME:
            draw_field();
            if (gotUpButtonPressed(1, true) && ticmem->cursor_y > 0) {
                ticmem->cursor_y--;
            }
            if (gotDownButtonPressed(1, true) && ticmem->cursor_y < 2) {
                ticmem->cursor_y++;
            }
            if (gotLeftButtonPressed(1, true) && ticmem->cursor_x > 0) {
                ticmem->cursor_x--;
            }
            if (gotRightButtonPressed(1, true) && ticmem->cursor_x < 2) {
                ticmem->cursor_x++;
            }
            if (gotPrimaryButtonPressed(1, false)) {
                if (ticmem->yourTurn && ticmem->board[ticmem->cursor_x][ticmem->cursor_y] == TICTACTOE_NONE) {
                    ticmem->board[ticmem->cursor_x][ticmem->cursor_y] = ticmem->isCross ? TICTACTOE_CROSS : TICTACTOE_CIRCLE;
                    ticmem->yourTurn = false;
                    uint8_t data[3];
                    data[0] = MSG_PLACE;
                    data[1] = ticmem->cursor_x;
                    data[2] = ticmem->cursor_y;
                    network_unicast(ticmem->otherPlayer, data, sizeof(data));
                    isGameOver(ticmem->isCross ? TICTACTOE_CROSS : TICTACTOE_CIRCLE);
                }
            }
            draw_cursor(ticmem->cursor_x,ticmem->cursor_y, ticmem->yourTurn);
            break;
        case TICTACTOE_GAME_END:
            if (gotPrimaryButtonPressed(1, false)) {
                ticmem->state = TICTACTOE_INGAME;
                for(int i=0;i<3;i++){
                    for(int j=0;j<3;j++){
                        TICMEM->board[i][j] = TICTACTOE_NONE;
                    }
                }
                uint8_t close = MSG_CLOSE;
                network_unicast(ticmem->otherPlayer, &close, 1);
            }
            if(if_winner(ticmem->isCross ? TICTACTOE_CROSS:TICTACTOE_CIRCLE)){
                get_graphics()->drawText(0,0,"won",0xFFFF);
                get_graphics()->drawText(0,10,ticmem->isCross ? "x":"o",0xFFFF);
            }
            if(check_if_full()){
                get_graphics()->drawText(0,0,"tie",0xFFFF);
            }
            break;
        
        default:
            break;
    }
}
void tictactoe_free(void* gamemem) {
    if (ticmem->state == TICTACTOE_INGAME) {
        uint8_t close = MSG_CLOSE;
        network_unicast(ticmem->otherPlayer, &close, 1);
    }
    network_uninit();
	free(gamemem);
}