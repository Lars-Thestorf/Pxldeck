#include "accounts.h"
#include <string.h>
#include <stdio.h>
#include <HLM_storage.h>

Accounts * acc_instanz = new Accounts();

Accounts* get_accounts() {
	return acc_instanz;
}

void Accounts::init(){

}
void Accounts::getName(char name[10], uint8_t id){
    strcpy(name,account[id].name);
}

void Accounts::setName(char name[10],uint8_t id){
    strcpy(account[id].name,name);
}

bool Accounts::createAccount(char name[10]){
    if(get_num_valid_accounts() == ACOOUNT_NUM){
        return false;
    }
    uint8_t acc_id = get_num_first_unused_account();
    strcpy(account[acc_id].name,name);
    account[acc_id].active = true;
    char key1[16];
    char key2[16];
    snprintf(key1,16,"accName%d",acc_id);
    printf("writestr %d",HLM_storage_write_str(key1,name));
    snprintf(key2,16,"accActive%d", acc_id);
    printf("writeID %d",HLM_storage_write32(key2,1));
    return true;
}
void Accounts::loadAccounts(){
    char key1[16];
    char key2[16];
    size_t size = 10;
    for(uint8_t i = 0;i < ACOOUNT_NUM;i++){
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
    strcpy(account[id].name,"------");
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