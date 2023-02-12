#ifndef _LALEMA_ACCOUNTS_H_
#define _LALEMA_ACCOUNTS_H_

#include <stdint.h>
#include <stdbool.h>

#define ACOOUNT_NUM 5

typedef struct account_t
{
    char name[10];
    bool active;
}account_t;

class Accounts{
public:
    uint8_t active_account;
    static void init();
    void setActiveAccount(uint8_t id);
    void setName(char* name);
    void setName(char* name, uint8_t id);
    void getName(char* name);
    bool createAccount(char* name);
    void loadAccounts();
    void deleteAccount(uint8_t id);
    uint8_t get_num_valid_accounts();

private:
    Accounts(){};
	account_t account[ACOOUNT_NUM];
    int8_t get_num_first_unused_account();
};

Accounts* get_accounts();

#endif