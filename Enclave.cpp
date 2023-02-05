#include "Enclave_t.h"

int generate_random_number()
{
    ocall_print("Processing random number generation...");
    return 42;
}

char generate_char()
{
    ocall_print("hey from generate char");
    return 's';
}