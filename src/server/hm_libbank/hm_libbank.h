#ifndef _LIBBANK_H
#define _LIBBANK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../../../hashmap/hashmap.h"
#include <stdint.h>

    struct baccount
    {
        int amount;
    };

    struct bank_ctx
    {
        struct hashmap *map;
    };

    struct bank_ctx *bank_create(int capacity);

    void account_create(struct bank_ctx *bank, const char *account_name);

    void account_delete(struct bank_ctx *bank, const char *account_name);

    int64_t amount_add(struct bank_ctx *bank, const char *account_name, int amount);

    int64_t amount_sub(struct bank_ctx *bank, const char *account_name, int amount);

    void list_accounts(struct bank_ctx *bank);

    // void bank_create(int capacity);

    // void account_create(const char *account_name);

    // void account_delete(const char *account_name);

    // int64_t amount_add(const char *account_name, int amount);

    // int64_t amount_sub(const char *account_name, int amount);

    // void list_accounts();

#ifdef __cplusplus
}
#endif
#endif //_LIBBANK_H
