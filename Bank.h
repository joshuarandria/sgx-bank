
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

struct bank_ctx
{
    struct hashmap *map;
};