#include <HLM_graphics.h>
#include <HLM_playerInput.h>
#include "settingsMenu.h"
#include <HLM_storage.h>
#include <stdio.h>

const char* brightness_storage_key = "Brightness";
uint8_t state_settings = 0;
bool in_settings_menu = true;
char name[12];

#define SETTINGS_STATE_NEW_ACC 0
#define SETTINGS_STATE_BRIGHTNESS 1
#define SETTINGS_STATE_MODE 2  

void DrawSettingsMenu(){
    char text_buffer[12];
    static uint8_t brightness = 5;
    static uint8_t character = 0;
    static uint8_t character_index = 0;
    HLM_graphics* gfx = get_graphics();
    gfx->clear();

    if(in_settings_menu){
        gfx->drawText(1,state_settings * 8,">",0xFFFF);
        gfx->drawText(8,0,"new acc",0xFFFF);
        gfx->drawText(8,8,"*5",0xFFFF);
        gfx->drawText(8,16,"SP",0xFFFF);

        if(getDInput(1,true) && state_settings < 2){
            state_settings++;
        }
        if(getUInput(1,true) && state_settings > 0){
            state_settings--;
        } 
        if(isPrimaryButtonPressed(1,true)){
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
            if(scrollDInput(1,500,50) && character < 27){
                character++;
            }
            if(scrollUInput(1,500,50) && character > 0){
                character--;
            }
            if(character == 0){
                name[character_index] = '<';
            }else{
                name[character_index] = character_index == 0 ? (character + 64) : (character + 96);
            }
            if(isPrimaryButtonPressed(1,true) && character_index < 10){
                if(character == 0){
                    in_settings_menu = true;
                }
                character_index++;
                character = 0;
            }
            if(isSecondaryButtonPressed(1,true) && character_index > 0){
                name[character_index] = ' ';
                character_index--;
                character = 0;
            }
            gfx->drawText(0,20,name,0xFFFF);
            break;
        case SETTINGS_STATE_BRIGHTNESS:
            if(scrollUInput(1,500,50) && brightness < 100){
                brightness++;
                gfx->setBrightness(brightness);
            }
            if(scrollDInput(1,500,50) && brightness > 1){
                brightness--;
                gfx->setBrightness(brightness);
            }
            gfx->drawText(0,0,"Brightness",0xFFFF);
            snprintf(text_buffer, 12, "* %d", brightness);
            gfx->drawText(0,10,text_buffer,0xFFFF);
            break;
        case SETTINGS_STATE_MODE:
            gfx->drawText(0,0,"Mode",0xFFFF);
            break;
        default:
            break;
        }
    }
    if(isPrimaryButtonPressed(1,true)){
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