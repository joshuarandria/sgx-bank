#ifndef ENCLAVE_U_H__
#define ENCLAVE_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_status_t etc. */

#include "Bank.h"

#include <stdlib.h> /* for size_t */

#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OCALL_PRINT_DEFINED__
#define OCALL_PRINT_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print, (const char* str));
#endif

sgx_status_t generate_random_number(sgx_enclave_id_t eid, int* retval);
sgx_status_t generate_char(sgx_enclave_id_t eid, char* retval);
sgx_status_t bank_create(sgx_enclave_id_t eid, int* retval, int* capacity);
sgx_status_t account_create(sgx_enclave_id_t eid, int* retval, const char* account_name);
sgx_status_t amount_add(sgx_enclave_id_t eid, int* retval, const char* account_name, int* amount);
sgx_status_t amount_sub(sgx_enclave_id_t eid, int* retval, const char* account_name, int* amount);
sgx_status_t list_accounts(sgx_enclave_id_t eid, int* retval);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
