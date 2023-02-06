#include <stdio.h>
#include "hm_libbank.h"

#include "../../../hashmap/hashmap.h"

#define DEBUG 0
int capacity, count = 0;

struct bank_ctx *bank_create(int capacity_loc)
{
    struct bank_ctx *bank = (struct bank_ctx *)malloc(sizeof(struct bank_ctx));
    if (!bank)
        printf("error bank==NULL");
    // return NULL;
    bank->map = new_hash_map(capacity_loc);
    printf("[%s] Bank created\n", __func__);
    capacity = capacity_loc;
    return bank;
}

void bank_destroy(struct bank_ctx *bank)
{
    printf("%s\n", __func__);
}

void account_create(struct bank_ctx *bank, const char *account_name)
{
#if DEBUG
    printf("\n\n-------------------CREATE n°%d------------------\n", count);
#endif
    char *account_name_copy = (char *)malloc(strlen((char *)account_name));

    strncpy(account_name_copy, (char *)account_name, strlen((char *)account_name) + 1);

    set_value(bank->map, account_name_copy, 0);
#if DEBUG
    printf("[%s]\n", __func__);
    printf("Account0 %s : hash %lu value %d\n", (char *)account_name, hashcode((char *)account_name, capacity), get_value(bank->map, (char *)account_name));
    printf("Account1 %s : hash %lu value %d\n", (char *)account_name_copy, hashcode((char *)account_name_copy, capacity), get_value(bank->map, (char *)account_name_copy));
#endif
    count++;
    // printf("%d", count);
    if (count % 200 == 0)
    {
        printf(" %d ", count);
    }
    if (count % 2000 == 0)
    {
        printf(" \n");
    }
    // list_accounts(bank);
}

// void account_delete(const char *account_name)
void account_delete(struct bank_ctx *bank, const char *account_name)
{
    printf("%s\n", __func__);
}
// int64_t amount_add(const char *account_name, int amount)
int64_t amount_add(struct bank_ctx *bank, const char *account_name, int amount)
{
#if DEBUG
    // printf("-----------ADD---------\n");
    int old = get_value(bank->map, (char *)account_name);
#endif
    int to_add = get_value(bank->map, (char *)account_name) + amount;
    set_value(bank->map, (char *)account_name, to_add);
#if DEBUG
    printf("[%s]: (%s): %d + %d = %d\n", __func__, account_name, old, amount, get_value(bank->map, (char *)account_name));
#endif
    return (1);
}

// int64_t amount_sub(const char *account_name, int amount)
int64_t amount_sub(struct bank_ctx *bank, const char *account_name, int amount)
{
#if DEBUG
    printf("-----------SUB---------\n");
    int old = get_value(bank->map, (char *)account_name);
#endif
    int to_sub = get_value(bank->map, (char *)account_name) - amount;
    set_value(bank->map, account_name, to_sub);
#if DEBUG
    printf("[%s]: (%s): %d - %d = %d\n", __func__, account_name, old, amount, get_value(bank->map, (char *)account_name));
#endif
    return (1);
}

void list_accounts(struct bank_ctx *bank)
{
#if DEBUG
    printf("\n-------Listing Accounts----------\n\n");
#endif
    FILE *filehashmap = fopen("hashmap.csv", "w");
    fprintf(filehashmap, "hash,key,value,next\n");
    for (int i = 0; i < capacity; i++)
        if (bank->map->list[i] != NULL)
        {
            printf("[%d] ", i);
            print_linkedlist(bank->map->list[i], filehashmap, i);
        }
#if DEBUG
    printf("\n\n-------END of Listing Accounts-------\n\n");
#endif
    fprintf(filehashmap, "\n");
    fclose(filehashmap);
}