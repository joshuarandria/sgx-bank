#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./hashmap.h"

struct hashmap *new_hash_map(int capacity)
{
    struct hashmap *local_hmap = (struct hashmap *)malloc(sizeof(struct hashmap));
    if (local_hmap == NULL)
    {
        printf("malloc failed\n");
        exit(1);
    }
    local_hmap->capacity = capacity;
    local_hmap->len = 0;
    local_hmap->list = (struct pair **)calloc(local_hmap->capacity, sizeof(struct pair *));
    if (local_hmap->list == NULL)
    {
        printf("calloc failed\n");
        exit(1);
    }
    printf("hm capacity = %d\n", local_hmap->capacity);
    return (local_hmap);
}

// djb2
unsigned long hashcode(const char *str, int cap)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % cap;
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
    printf("new hashcode for the key %s !", key);
    return (-1);
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
    if (!new_pair)
    {
        perror("malloc for new pair failed");
    }
    new_pair->key = (char *)key;
    new_pair->val = val;
    // we set the new_pair.next to the beginning of the linked list of this index
    new_pair->next = hm->list[index];
    // and after we got the next of list[index] we re attach it to our new_pair.
    hm->list[index] = new_pair;
    // we increment the total of element in the hashmap
    hm->len++;
}

void print_linkedlist(struct pair *cur, FILE *filehashmap, int hash)
{
    printf("(%s,%d) -> ", cur->key, cur->val);
    if (cur->next)
        fprintf(filehashmap, "%d,%s,%d,%s\n", hash, cur->key, cur->val, cur->next->key);
    else
        fprintf(filehashmap, "%d,%s,%d,NULL\n", hash, cur->key, cur->val);
    while (cur->next)
    {
        cur = cur->next;
        printf("(%s,%d) -> ", cur->key, cur->val);
        if (cur->next)
            fprintf(filehashmap, "%d,%s,%d,%s\n", hash, cur->key, cur->val, cur->next->key);
        else
            fprintf(filehashmap, "%d,%s,%d,NULL\n", hash, cur->key, cur->val);
    }
    printf("NULL\n");
    printf("\n");
    fprintf(filehashmap, "\n");
    return;
}

// int main()
// {
//     struct hashmap *hm = new_hash_map(2);
//     set_value(hm, "eloi", 16);
//     set_value(hm, "eloai2", 11);
//     set_value(hm, "eloic3", 12);
//     set_value(hm, "esloi4", 101);
//     set_value(hm, "eloi5", 102);
//     set_value(hm, "elossi6", 103);
//     set_value(hm, "eloi7", 104);
//     set_value(hm, "eloi8", 13);
//     set_value(hm, "eloi9", 105);
//     print_linkedlist(hm->list[0]);
//     print_linkedlist(hm->list[1]);
// }