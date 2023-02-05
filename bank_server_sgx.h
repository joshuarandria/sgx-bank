#include "Enclave_u.h"
#include "sgx_urts.h"

/* Global EID shared by multiple threads */
extern sgx_enclave_id_t global_eid;
extern int retval;