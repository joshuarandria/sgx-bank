#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef HASHMAP
#define HASHMAP
struct pair
{
    char *key;
    int val;
    struct pair *next;
};

struct hashmap
{
    struct pair **list;
    int capacity; // capacity of hashmap
    int len;      // number of pairs in hashmap (could be more than capacity if the values have collided hashes)
};

struct hashmap *new_hash_map(int capacity);

// djb2
unsigned long hashcode(const char *str, int cap);

int get_value(struct hashmap *hm, char *key);

void set_value(struct hashmap *hm, const char *key, int val);

void print_linkedlist(struct pair *head, FILE *filehashmap, int hash);

#endif