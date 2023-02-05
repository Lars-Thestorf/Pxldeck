#include <HLM_graphics.h>
#include <HLM_playerInput.h>
#include "settingsMenu.h"
#include <HLM_storage.h>
#include <stdio.h>
#include <defaultInputEvents.h>

const char* brightness_storage_key = "Brightness";
uint8_t state_settings = 0;
bool in_settings_menu = true;
char name[12];
//TODO move to mem struct

#define SETTINGS_STATE_NEW_ACC 0
#define SETTINGS_STATE_BRIGHTNESS 1
#define SETTINGS_STATE_MODE 2  

void DrawSettingsMenu(){
    char text_buffer[12];
    static uint8_t brightness = 0;
    static uint8_t character = 0;
    static uint8_t character_index = 0;
    HLM_graphics* gfx = get_graphics();
    gfx->clear();

    if(in_settings_menu){
        if (brightness == 0) {
            brightness = 5;
            if (HLM_storage_exists32(brightness_storage_key)) {
                brightness = HLM_storage_read32(brightness_storage_key);
            }
        }

        gfx->drawText(1,state_settings * 8,">",0xFFFF);
        gfx->drawText(8,0,"new acc",0xFFFF);
        snprintf(text_buffer, 12, "* %d", brightness);
        gfx->drawText(8,8,text_buffer,0xFFFF);
        gfx->drawText(8,16,"SP",0xFFFF);

        if (gotDownButtonPressed(1, true)) {
            if (state_settings < 2)
                state_settings++;
        }
        if (gotUpButtonPressed(1, true)) {
            if (state_settings > 0)
                state_settings--;
        }
        if(gotPrimaryButtonPressed(1, false)){
            in_settings_menu = false;
            switch (state_settings){
            case SETTINGS_STATE_NEW_ACC:
                snprintf(name,12,"           ");
                character = 0;
                character_index = 0;
                break;
            case SETTINGS_STATE_BRIGHTNESS:
                brightness = 5;
                if (HLM_storage_exists32(brightness_storage_key)) {
		            brightness = HLM_storage_read32(brightness_storage_key);
	            }
                break;
            case SETTINGS_STATE_MODE:

                break;
            
            default:
                break;
            }
        }    
    }else{
        switch (state_settings){
        case SETTINGS_STATE_NEW_ACC:
            gfx->drawText(0,0,"New Acc",0xFFFF);
            gfx->drawText(0,8,"Name:",0xFFFF);
            if(gotUpButtonPressed(1, true) && character < 27){
                character++;
            }
            if(gotDownButtonPressed(1, true) && character > 0){
                character--;
            }
            if(character == 0){
                name[character_index] = '<';
            }else{
                name[character_index] = character_index == 0 ? (character + 64) : (character + 96);
            }
            if(gotPrimaryButtonPressed(1, false) && character_index < 10){
                if(character == 0){
                    in_settings_menu = true;
                }
                character_index++;
                character = 0;
            }
            if(gotSecondaryButtonPressed(1, false) && character_index > 0){
                name[character_index] = ' ';
                character_index--;
                character = 0;
            }
            gfx->drawText(0,20,name,0xFFFF);
            break;
        case SETTINGS_STATE_BRIGHTNESS:
            if(gotUpButtonPressed(1, true) && brightness < 100){
                brightness++;
                gfx->setBrightness(brightness);
            }
            if(gotDownButtonPressed(1, true) && brightness > 1){
                brightness--;
                gfx->setBrightness(brightness);
            }
            gfx->drawText(0,0,"Brightness",0xFFFF);
            snprintf(text_buffer, 12, "* %d", brightness);
            gfx->drawText(0,10,text_buffer,0xFFFF);
            break;
        case SETTINGS_STATE_MODE:
            gfx->drawText(0,0,"Mode",0xFFFF);
            if (gotUpButtonPressed(1, false)) {
                cycleInputMethods();
            }
            for (uint8_t i = 1; i <= 3; i++)
            switch(getControllerType(i)) {
                case CONTROLLERTYPE_FULL:
                    gfx->drawText(0,i*8,"Full",0xFFFF);
                    break;
                case CONTROLLERTYPE_LEFT:
                    gfx->drawText(0,i*8,"Left",0xFFFF);
                    break;
                case CONTROLLERTYPE_RIGHT:
                    gfx->drawText(0,i*8,"Right",0xFFFF);
                    break;
                case CONTROLLERTYPE_KEYBOARD:
                    gfx->drawText(0,i*8,"Keyboard",0xFFFF);
                    break;
                case CONTROLLERTYPE_EXT:
                    gfx->drawText(0,i*8,"Ext",0xFFFF);
                    break;
                case CONTROLLERTYPE_NONE:
                    break;
            }
            break;
        default:
            break;
        }
    }
    if(gotPrimaryButtonPressed(1, false)){
        in_settings_menu = true;
        switch (state_settings)
        {
        case SETTINGS_STATE_NEW_ACC:
            
            break;
        case SETTINGS_STATE_BRIGHTNESS:
            HLM_storage_write32(brightness_storage_key, brightness);
            break;
        case SETTINGS_STATE_MODE:

            break;
        
        default:
            break;
        }
    }
}