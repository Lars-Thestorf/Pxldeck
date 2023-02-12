#include <HLM_graphics.h>
#include <HLM_playerInput.h>
#include "settingsMenu.h"
#include <HLM_storage.h>
#include <stdio.h>
#include <cstdio>
#include <defaultInputEvents.h>
#include "../accounts/accounts.h"

typedef struct settings_mem_t {
	uint8_t state_settings;
    bool in_settings_menu;
    uint8_t brightness;
    uint8_t initial_brightness;
    char name[10];
    uint8_t character;
    uint8_t index;
} settings_mem_t;

const char* brightness_storage_key = "Brightness";
settings_mem_t *pointer;

#define SETTINGS_STATE_NEW_ACC 0
#define SETTINGS_STATE_DELETE_ACC 1
#define SETTINGS_STATE_BRIGHTNESS 2
#define SETTINGS_STATE_MODE 3  

void setting_menu_init(){
    pointer = (settings_mem_t*)malloc(sizeof(settings_mem_t));
    pointer->brightness = 5;
    pointer->state_settings = 0 ; 
    pointer->in_settings_menu = true;
    if (HLM_storage_exists32(brightness_storage_key)) {
        pointer->brightness = HLM_storage_read32(brightness_storage_key);
        pointer->initial_brightness = pointer->brightness;
    }   
}

void settings_menu_free(){
    free(pointer);
}
void DrawSettingsMenu(){
    char text_buffer[12];
    HLM_graphics* gfx = get_graphics();
    Accounts* acc = get_accounts();
    gfx->clear();

    if(pointer->in_settings_menu){    
        gfx->drawText(1,pointer->state_settings * 8,">",0xFFFF);
        gfx->drawText(8,0,"Account+",0xFFFF);
        gfx->drawText(8,8,"Account-",0xFFFF);
        snprintf(text_buffer, 12, "* %d", pointer->brightness);
        gfx->drawText(8,16,text_buffer,0xFFFF);
        gfx->drawText(8,24,"Mode",0xFFFF);

        if (gotDownButtonPressed(1, true)) {
            if (pointer->state_settings < 3)
                pointer->state_settings++;
        }
        if (gotUpButtonPressed(1, true)) {
            if (pointer->state_settings > 0)
                pointer->state_settings--;
        }
        if(gotPrimaryButtonPressed(1, false)){
            pointer->in_settings_menu = false;
            switch (pointer->state_settings){
            case SETTINGS_STATE_NEW_ACC:
                snprintf(pointer->name,10,"         ");
                pointer->character = 0;
                pointer->index = 0;
                break;
            case SETTINGS_STATE_DELETE_ACC:
                pointer->index = 0;
                break;
            case SETTINGS_STATE_BRIGHTNESS:
    
                break;
            case SETTINGS_STATE_MODE:

                break;
            
            default:
                break;
            }
        }    
    }else{
        switch (pointer->state_settings){
        case SETTINGS_STATE_NEW_ACC:
            gfx->drawText(0,0,"Account+",0xFFFF);
            if(acc->get_num_valid_accounts() == ACOOUNT_NUM){
                gfx->drawText(0,8,"No Space",0xFFFF);
                break;
            }else{
                gfx->drawText(0,8,"Name:",0xFFFF);
            }
            if(gotUpButtonPressed(1, true) && pointer->character < 26){
                pointer->character++;
            }
            if(gotDownButtonPressed(1, true) && pointer->character > 0){
                pointer->character--;
            }
            if(pointer->index == 0 && pointer->character == 0){
                pointer->character = 1;
            }
            if(pointer->index == 7){
                pointer->character = 0;
            }
            if(pointer->character == 0){
                pointer->name[pointer->index] = '<';
            }else{
                pointer->name[pointer->index] = pointer->index == 0 ? (pointer->character + 64) : (pointer->character + 96);
            }
            if(gotPrimaryButtonPressed(1, false)){
                if(pointer->character == 0){
                    pointer->in_settings_menu = true;
                    pointer->name[pointer->index] = '\0';
                    acc->createAccount(pointer->name);
                }
                pointer->index++;
                pointer->character = 0;
            }
            if(gotSecondaryButtonPressed(1, false)){
                if(pointer->index > 0){
                    pointer->name[pointer->index] = ' ';
                    pointer->index--;
                    pointer->character = 0;
                }else{
                    pointer->in_settings_menu= true;
                }
            }
            gfx->drawText(0,20,pointer->name,0xFFFF);
            break;
        case SETTINGS_STATE_DELETE_ACC:
            gfx->drawText(0,0,"Account-",0xFFFF);
            gfx->drawText(0,8,"Accounts:",0xFFFF);
            acc->getName(pointer->name);
            snprintf(text_buffer, 15, "%d. %s", acc->active_account + 1,pointer->name);
            gfx->drawText(0,20,text_buffer,0xFFFF);
            if(gotDownButtonPressed(1, true) && acc->active_account < ACOOUNT_NUM - 1){
                acc->active_account++;
            }
            if(gotUpButtonPressed(1, true) && acc->active_account > 0){
                acc->active_account--;
            }
            if(gotPrimaryButtonPressed(1, false)){
                acc->deleteAccount(acc->active_account);
            }
            if(gotSecondaryButtonPressed(1, false)){
                pointer->in_settings_menu= true;
            }
            break;
        case SETTINGS_STATE_BRIGHTNESS:
            if((gotUpButtonPressed(1, true) || gotRightButtonPressed(1,true)) && pointer->brightness < 100){
                pointer->brightness++;
                gfx->setBrightness(pointer->brightness);
            }
            if((gotDownButtonPressed(1, true) || gotLeftButtonPressed(1,true)) && pointer->brightness > 1){
                pointer->brightness--;
                gfx->setBrightness(pointer->brightness);
            }
            if(gotSecondaryButtonPressed(1, false)){
                pointer->in_settings_menu= true;
                pointer->brightness = pointer->initial_brightness;
                gfx->setBrightness(pointer->initial_brightness);
            }
            gfx->drawText(0,0,"Brightness",0xFFFF);
            snprintf(text_buffer, 12, "* %d", pointer->brightness);
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
        pointer->in_settings_menu = true;
        switch (pointer->state_settings)
        {
        case SETTINGS_STATE_NEW_ACC:
            
            break;
        case SETTINGS_STATE_DELETE_ACC:

            break;
        case SETTINGS_STATE_BRIGHTNESS:
            HLM_storage_write32(brightness_storage_key, pointer->brightness);
            break;
        case SETTINGS_STATE_MODE:

            break;
        
        default:
            break;
        }
    }
}