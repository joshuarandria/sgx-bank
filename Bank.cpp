#include "sgx_trts.h"
#include "Enclave_t.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define DEBUG 1

int count = 0;
struct bank_ctx *bank;
int *capacity_local = 0;

void man_printf(const char *msg, ...)
{
    va_list va;
    va_start(va, msg);
    char buf[4096];
    vsnprintf(buf, 4096, msg, va);
    va_end(va);
    ocall_print(buf);
}

void print_linkedlist(struct pair *cur)
{
    man_printf("(%s,%d) -> ", cur->key, cur->val);
    while (cur->next)
    {
        cur = cur->next;
        man_printf("(%s,%d) -> ", cur->key, cur->val);
    }
    man_printf("NULL\n");
    man_printf("\n");
    return;
}

int list_accounts()
{
    for (int i = 0; i < bank->map->capacity; i++)
        if (bank->map->list[i] != NULL)
        {
            print_linkedlist(bank->map->list[i]);
        }
    return 1;
}
struct hashmap *new_hash_map(int capacity)
{
    struct hashmap *local_hmap = (struct hashmap *)malloc(sizeof(struct hashmap));
    local_hmap->capacity = capacity;
    local_hmap->len = 0;
    local_hmap->list = (struct pair **)calloc(local_hmap->capacity, sizeof(struct pair *));
    return (local_hmap);
}

unsigned long hashcode(const char *str, int cap)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % cap;
}

void set_value(struct hashmap *hm, const char *key, int val)
{
    // if value is already in hashmap
    struct pair *cur;
    int index = hashcode(key, hm->capacity);

    for (cur = hm->list[index]; cur; cur = cur->next)
    {
        if (!strncmp(cur->key, key, strlen(key)))
        {
            cur->val = val;
            return;
        }
    }
    // if key isn't already in hashmap, we add it and head points towards the last inserted element
    // let a,b,c and head = list[index];
    // first we have head->NULL;
    // 1.set(a),2.set(b),3.set(c) will make head->c->b->a->NULL;
    struct pair *new_pair = (struct pair *)malloc(sizeof(struct pair));
    new_pair->key = (char *)key;
    new_pair->val = val;
    // we set the new_pair.next to the beginning of the linked list of this index
    new_pair->next = hm->list[index];
    // and after we got the next of list[index] we re attach it to our new_pair.
    hm->list[index] = new_pair;
    // we increment the total of element in the hashmap
    hm->len++;
}

int get_value(struct hashmap *hm, char *key)
{
    struct pair *cur;

    for (cur = hm->list[hashcode(key, hm->capacity)]; cur; cur = cur->next)
    {
        if (!strncmp(cur->key, key, strlen(key)))
        {
            return (cur->val);
        }
    }
    return (-1);
}

int bank_create(int *capacity)
{
    bank = (struct bank_ctx *)malloc(sizeof(struct bank_ctx));
    bank->map = new_hash_map(*capacity);
    if (!bank->map)
        return -1;
    capacity_local = capacity;
    return 1;
}

int account_create(const char *account_name)
{
    char *account_name_copy = (char *)malloc(strlen((char *)account_name));
    if (!account_name_copy)
        return -1;
    strncpy(account_name_copy, (char *)account_name, strlen((char *)account_name) + 1);
    set_value(bank->map, account_name_copy, 0);
    // man_printf("Account %s : value %d\n", (char *)account_name_copy, get_value(bank->map, (char *)account_name_copy));
    count++;
    if (count % 200 == 0)
    {
        man_printf(" %d ", count);
    }
    if (count % 2000 == 0)
    {
        man_printf(" \n");
    }
    return 1;
}
int amount_add(const char *account_name, int *amount)
{
    int to_add = get_value(bank->map, (char *)account_name) + *amount;
    set_value(bank->map, (char *)account_name, to_add);
    return 1;
}
int amount_sub(const char *account_name, int *amount)
{
    int old = get_value(bank->map, (char *)account_name);
    int to_sub = get_value(bank->map, (char *)account_name) - *amount;
    set_value(bank->map, account_name, to_sub);
    return 1;
}
