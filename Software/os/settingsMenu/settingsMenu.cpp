#include <HLM_graphics.h>
#include <HLM_playerInput.h>
#include "settingsMenu.h"


void DrawSettingsMenu(){
    HLM_graphics* gfx = get_graphics();
    gfx->drawText(1,0,">",0xFFFF);
    gfx->drawText(8,0,"new acc",0xFFFF);
    gfx->drawText(8,8,"*5",0xFFFF);
    gfx->drawText(8,16,"SP",0xFFFF);
    gfx->drawText(8,24,"Search",0xFFFF);
}