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
    void init();
    void setName(char name[10], uint8_t id);
    void getName(char name[10],uint8_t id);
    bool createAccount(char name[10]);
    void loadAccounts();
    void deleteAccount(uint8_t id);
    uint8_t get_num_valid_accounts();

private:
	account_t account[ACOOUNT_NUM];
    int8_t get_num_first_unused_account();
};

Accounts* get_accounts();

#endif