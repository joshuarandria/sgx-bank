#include "Enclave_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */
#include "sgx_lfence.h" /* for sgx_lfence */

#include <errno.h>
#include <mbusafecrt.h> /* for memcpy_s etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_ENCLAVE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_within_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define ADD_ASSIGN_OVERFLOW(a, b) (	\
	((a) += (b)) < (b)	\
)


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

static sgx_status_t SGX_CDECL sgx_generate_random_number(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_generate_random_number_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_generate_random_number_t* ms = SGX_CAST(ms_generate_random_number_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	int _in_retval;


	_in_retval = generate_random_number();
	if (MEMCPY_S(&ms->ms_retval, sizeof(ms->ms_retval), &_in_retval, sizeof(_in_retval))) {
		status = SGX_ERROR_UNEXPECTED;
		goto err;
	}

err:
	return status;
}

static sgx_status_t SGX_CDECL sgx_generate_char(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_generate_char_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_generate_char_t* ms = SGX_CAST(ms_generate_char_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	char _in_retval;


	_in_retval = generate_char();
	if (MEMCPY_S(&ms->ms_retval, sizeof(ms->ms_retval), &_in_retval, sizeof(_in_retval))) {
		status = SGX_ERROR_UNEXPECTED;
		goto err;
	}

err:
	return status;
}

static sgx_status_t SGX_CDECL sgx_bank_create(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_bank_create_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_bank_create_t* ms = SGX_CAST(ms_bank_create_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	int* _tmp_capacity = ms->ms_capacity;
	size_t _len_capacity = sizeof(int);
	int* _in_capacity = NULL;
	int _in_retval;

	CHECK_UNIQUE_POINTER(_tmp_capacity, _len_capacity);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_capacity != NULL && _len_capacity != 0) {
		if ( _len_capacity % sizeof(*_tmp_capacity) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_capacity = (int*)malloc(_len_capacity);
		if (_in_capacity == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_capacity, _len_capacity, _tmp_capacity, _len_capacity)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	_in_retval = bank_create(_in_capacity);
	if (MEMCPY_S(&ms->ms_retval, sizeof(ms->ms_retval), &_in_retval, sizeof(_in_retval))) {
		status = SGX_ERROR_UNEXPECTED;
		goto err;
	}

err:
	if (_in_capacity) free(_in_capacity);
	return status;
}

static sgx_status_t SGX_CDECL sgx_account_create(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_account_create_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_account_create_t* ms = SGX_CAST(ms_account_create_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	const char* _tmp_account_name = ms->ms_account_name;
	size_t _len_account_name = ms->ms_account_name_len ;
	char* _in_account_name = NULL;
	int _in_retval;

	CHECK_UNIQUE_POINTER(_tmp_account_name, _len_account_name);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_account_name != NULL && _len_account_name != 0) {
		_in_account_name = (char*)malloc(_len_account_name);
		if (_in_account_name == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_account_name, _len_account_name, _tmp_account_name, _len_account_name)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

		_in_account_name[_len_account_name - 1] = '\0';
		if (_len_account_name != strlen(_in_account_name) + 1)
		{
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}
	_in_retval = account_create((const char*)_in_account_name);
	if (MEMCPY_S(&ms->ms_retval, sizeof(ms->ms_retval), &_in_retval, sizeof(_in_retval))) {
		status = SGX_ERROR_UNEXPECTED;
		goto err;
	}

err:
	if (_in_account_name) free(_in_account_name);
	return status;
}

static sgx_status_t SGX_CDECL sgx_amount_add(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_amount_add_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_amount_add_t* ms = SGX_CAST(ms_amount_add_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	const char* _tmp_account_name = ms->ms_account_name;
	size_t _len_account_name = ms->ms_account_name_len ;
	char* _in_account_name = NULL;
	int* _tmp_amount = ms->ms_amount;
	size_t _len_amount = sizeof(int);
	int* _in_amount = NULL;
	int _in_retval;

	CHECK_UNIQUE_POINTER(_tmp_account_name, _len_account_name);
	CHECK_UNIQUE_POINTER(_tmp_amount, _len_amount);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_account_name != NULL && _len_account_name != 0) {
		_in_account_name = (char*)malloc(_len_account_name);
		if (_in_account_name == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_account_name, _len_account_name, _tmp_account_name, _len_account_name)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

		_in_account_name[_len_account_name - 1] = '\0';
		if (_len_account_name != strlen(_in_account_name) + 1)
		{
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}
	if (_tmp_amount != NULL && _len_amount != 0) {
		if ( _len_amount % sizeof(*_tmp_amount) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_amount = (int*)malloc(_len_amount);
		if (_in_amount == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_amount, _len_amount, _tmp_amount, _len_amount)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	_in_retval = amount_add((const char*)_in_account_name, _in_amount);
	if (MEMCPY_S(&ms->ms_retval, sizeof(ms->ms_retval), &_in_retval, sizeof(_in_retval))) {
		status = SGX_ERROR_UNEXPECTED;
		goto err;
	}

err:
	if (_in_account_name) free(_in_account_name);
	if (_in_amount) free(_in_amount);
	return status;
}

static sgx_status_t SGX_CDECL sgx_amount_sub(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_amount_sub_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_amount_sub_t* ms = SGX_CAST(ms_amount_sub_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	const char* _tmp_account_name = ms->ms_account_name;
	size_t _len_account_name = ms->ms_account_name_len ;
	char* _in_account_name = NULL;
	int* _tmp_amount = ms->ms_amount;
	size_t _len_amount = sizeof(int);
	int* _in_amount = NULL;
	int _in_retval;

	CHECK_UNIQUE_POINTER(_tmp_account_name, _len_account_name);
	CHECK_UNIQUE_POINTER(_tmp_amount, _len_amount);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_account_name != NULL && _len_account_name != 0) {
		_in_account_name = (char*)malloc(_len_account_name);
		if (_in_account_name == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_account_name, _len_account_name, _tmp_account_name, _len_account_name)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

		_in_account_name[_len_account_name - 1] = '\0';
		if (_len_account_name != strlen(_in_account_name) + 1)
		{
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}
	if (_tmp_amount != NULL && _len_amount != 0) {
		if ( _len_amount % sizeof(*_tmp_amount) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_amount = (int*)malloc(_len_amount);
		if (_in_amount == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_amount, _len_amount, _tmp_amount, _len_amount)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	_in_retval = amount_sub((const char*)_in_account_name, _in_amount);
	if (MEMCPY_S(&ms->ms_retval, sizeof(ms->ms_retval), &_in_retval, sizeof(_in_retval))) {
		status = SGX_ERROR_UNEXPECTED;
		goto err;
	}

err:
	if (_in_account_name) free(_in_account_name);
	if (_in_amount) free(_in_amount);
	return status;
}

static sgx_status_t SGX_CDECL sgx_list_accounts(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_list_accounts_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_list_accounts_t* ms = SGX_CAST(ms_list_accounts_t*, pms);
	sgx_status_t status = SGX_SUCCESS;
	int _in_retval;


	_in_retval = list_accounts();
	if (MEMCPY_S(&ms->ms_retval, sizeof(ms->ms_retval), &_in_retval, sizeof(_in_retval))) {
		status = SGX_ERROR_UNEXPECTED;
		goto err;
	}

err:
	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* ecall_addr; uint8_t is_priv; uint8_t is_switchless;} ecall_table[7];
} g_ecall_table = {
	7,
	{
		{(void*)(uintptr_t)sgx_generate_random_number, 0, 0},
		{(void*)(uintptr_t)sgx_generate_char, 0, 0},
		{(void*)(uintptr_t)sgx_bank_create, 0, 0},
		{(void*)(uintptr_t)sgx_account_create, 0, 0},
		{(void*)(uintptr_t)sgx_amount_add, 0, 0},
		{(void*)(uintptr_t)sgx_amount_sub, 0, 0},
		{(void*)(uintptr_t)sgx_list_accounts, 0, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[1][7];
} g_dyn_entry_table = {
	1,
	{
		{0, 0, 0, 0, 0, 0, 0, },
	}
};


sgx_status_t SGX_CDECL ocall_print(const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_ocall_print_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(str, _len_str);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (str != NULL) ? _len_str : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_t));
	ocalloc_size -= sizeof(ms_ocall_print_t);

	if (str != NULL) {
		if (MEMCPY_S(&ms->ms_str, sizeof(const char*), &__tmp, sizeof(const char*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		if (_len_str % sizeof(*str) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (MEMCPY_S(__tmp, ocalloc_size, str, _len_str)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_str);
		ocalloc_size -= _len_str;
	} else {
		ms->ms_str = NULL;
	}

	status = sgx_ocall(0, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

