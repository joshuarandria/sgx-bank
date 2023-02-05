#include "Enclave_u.h"
#include <errno.h>

typedef struct ms_generate_random_number_t {
	int ms_retval;
} ms_generate_random_number_t;

typedef struct ms_generate_char_t {
	char ms_retval;
} ms_generate_char_t;

typedef struct ms_bank_create_t {
	int ms_retval;
	int* ms_capacity;
} ms_bank_create_t;

typedef struct ms_account_create_t {
	int ms_retval;
	const char* ms_account_name;
	size_t ms_account_name_len;
} ms_account_create_t;

typedef struct ms_amount_add_t {
	int ms_retval;
	const char* ms_account_name;
	size_t ms_account_name_len;
	int* ms_amount;
} ms_amount_add_t;

typedef struct ms_amount_sub_t {
	int ms_retval;
	const char* ms_account_name;
	size_t ms_account_name_len;
	int* ms_amount;
} ms_amount_sub_t;

typedef struct ms_list_accounts_t {
	int ms_retval;
} ms_list_accounts_t;

typedef struct ms_ocall_print_t {
	const char* ms_str;
} ms_ocall_print_t;

static sgx_status_t SGX_CDECL Enclave_ocall_print(void* pms)
{
	ms_ocall_print_t* ms = SGX_CAST(ms_ocall_print_t*, pms);
	ocall_print(ms->ms_str);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * table[1];
} ocall_table_Enclave = {
	1,
	{
		(void*)Enclave_ocall_print,
	}
};
sgx_status_t generate_random_number(sgx_enclave_id_t eid, int* retval)
{
	sgx_status_t status;
	ms_generate_random_number_t ms;
	status = sgx_ecall(eid, 0, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t generate_char(sgx_enclave_id_t eid, char* retval)
{
	sgx_status_t status;
	ms_generate_char_t ms;
	status = sgx_ecall(eid, 1, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t bank_create(sgx_enclave_id_t eid, int* retval, int* capacity)
{
	sgx_status_t status;
	ms_bank_create_t ms;
	ms.ms_capacity = capacity;
	status = sgx_ecall(eid, 2, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t account_create(sgx_enclave_id_t eid, int* retval, const char* account_name)
{
	sgx_status_t status;
	ms_account_create_t ms;
	ms.ms_account_name = account_name;
	ms.ms_account_name_len = account_name ? strlen(account_name) + 1 : 0;
	status = sgx_ecall(eid, 3, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t amount_add(sgx_enclave_id_t eid, int* retval, const char* account_name, int* amount)
{
	sgx_status_t status;
	ms_amount_add_t ms;
	ms.ms_account_name = account_name;
	ms.ms_account_name_len = account_name ? strlen(account_name) + 1 : 0;
	ms.ms_amount = amount;
	status = sgx_ecall(eid, 4, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t amount_sub(sgx_enclave_id_t eid, int* retval, const char* account_name, int* amount)
{
	sgx_status_t status;
	ms_amount_sub_t ms;
	ms.ms_account_name = account_name;
	ms.ms_account_name_len = account_name ? strlen(account_name) + 1 : 0;
	ms.ms_amount = amount;
	status = sgx_ecall(eid, 5, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

sgx_status_t list_accounts(sgx_enclave_id_t eid, int* retval)
{
	sgx_status_t status;
	ms_list_accounts_t ms;
	status = sgx_ecall(eid, 6, &ocall_table_Enclave, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

