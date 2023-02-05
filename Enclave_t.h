#ifndef ENCLAVE_T_H__
#define ENCLAVE_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */

#include "Bank.h"

#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NO_HARDEN_EXT_WRITES
#define MEMCPY_S memcpy_s
#define MEMSET memset
#else
#define MEMCPY_S memcpy_verw_s
#define MEMSET memset_verw
#endif /* NO_HARDEN_EXT_WRITES */

int generate_random_number(void);
char generate_char(void);
int bank_create(int* capacity);
int account_create(const char* account_name);
int amount_add(const char* account_name, int* amount);
int amount_sub(const char* account_name, int* amount);
int list_accounts(void);

sgx_status_t SGX_CDECL ocall_print(const char* str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
