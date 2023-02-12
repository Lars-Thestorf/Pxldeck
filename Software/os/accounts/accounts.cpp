#include "accounts.h"
#include <stdio.h>
#include <HLM_storage.h>

Accounts * acc_instanz = NULL;

Accounts* get_accounts() {
	return acc_instanz;
}

void Accounts::init(){
    if(acc_instanz != NULL)
        return;
    acc_instanz = new Accounts();
    acc_instanz->setActiveAccount(0);
}
void Accounts::setActiveAccount(uint8_t id){
    active_account = id;
}
void Accounts::getName(char* name){
    snprintf(name,10,"%s",account[active_account].name);
}

void Accounts::setName(char* name){
    snprintf(account[active_account].name,10,"%s",name);
}
void Accounts::setName(char* name,uint8_t id){
    snprintf(account[id].name,10,"%s",name);
}

bool Accounts::createAccount(char* name){
    if(get_num_valid_accounts() == ACOOUNT_NUM){
        return false;
    }
    uint8_t acc_id = get_num_first_unused_account();
    snprintf(account[acc_id].name,10,"%s",name);
    account[acc_id].active = true;
    char key1[16];
    char key2[16];
    snprintf(key1,16,"accName%d",acc_id);
    printf("writestr %d",HLM_storage_write_str(key1,account[acc_id].name));
    snprintf(key2,16,"accActive%d", acc_id);
    printf("writeID %d",HLM_storage_write32(key2,1));
    return true;
}
void Accounts::loadAccounts(){
    char key1[16];
    char key2[16];
    size_t size;
    for(uint8_t i = 0;i < ACOOUNT_NUM;i++){
        size = 10;
        snprintf(key1,16,"accActive%d", i);
        account[i].active = HLM_storage_read32(key1);
        printf("getactive %d",account[i].active);
        if(account[i].active){
            snprintf(key2,16,"accName%d",i);
            printf("reedstr %d",HLM_storage_read_str(key2,account[i].name,&size));  
        }else{
            snprintf(account[i].name,10,"------");
        }
    }
}
void Accounts::deleteAccount(uint8_t id){
    snprintf(account[id].name,10,"------");
    account[id].active = false;
    char key[16];
    snprintf(key,16,"accActive%d", id);
    HLM_storage_write32(key,0);
}
uint8_t Accounts::get_num_valid_accounts(){
    uint8_t valid_accounts = 0;
    for(uint8_t i = 0;i < ACOOUNT_NUM;i++){
        if(account[i].active == true){
            valid_accounts++;
        }
    }
    return valid_accounts;
}
int8_t Accounts::get_num_first_unused_account(){
    for(uint8_t i = 0;i < ACOOUNT_NUM;i++){
        if(account[i].active == false){
            return i;
        }
    }
    return -1;
}