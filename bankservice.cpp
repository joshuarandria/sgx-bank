#include "bankservice.h"
#include "bank_server_sgx.h"

void BankServiceImpl::BankCreate(int64_t cap)
{
    int *ptr_cap = (int *)&cap;
    // printf("[%s]\n", __func__);
    bank_create(global_eid, &retval, ptr_cap);
}

grpc::Status
BankServiceImpl::CreateAccount(grpc::ServerContext *context,
                               const bank::AccountCreateReq *request,
                               bank::AccountCreateResp *response)
{
    // printf("[%s]\n", __func__);
    const char *account_name = request->account_name().c_str();
    // printf("[bankservice] CreateAccount %s \n", account_name);
    account_create(global_eid, &retval, account_name);

    response->set_new_amount(0);
    return grpc::Status::OK;
}

grpc::Status
BankServiceImpl::AddAmount(grpc::ServerContext *context,
                           const bank::AmountAddReq *request,
                           bank::AmountAddResp *response)
{
    // printf("[%s] de bankservice \n", __func__);

    const char *account_name = request->account_name().c_str();
    int amount = request->amount();
    int *ptr_amount = (int *)&amount;

    int new_amount = amount_add(global_eid, &retval, account_name, ptr_amount);
    response->set_new_amount(new_amount);
    return grpc::Status::OK;
}

grpc::Status
BankServiceImpl::SubAmount(grpc::ServerContext *context,
                           const bank::AmountSubReq *request,
                           bank::AmountSubResp *response)
{
    // printf("[%s] de bankservice \n", __func__);

    const char *account_name = request->account_name().c_str();
    int amount = request->amount();
    int *ptr_amount = (int *)&amount;
    int new_amount = amount_sub(global_eid, &retval, account_name, ptr_amount);

    response->set_new_amount(new_amount);

    return grpc::Status::OK;
}

grpc::Status
BankServiceImpl::ListAccount(grpc::ServerContext *context,
                             const bank::Empty *request,
                             bank::Empty *response)
{
    // printf("[%s] de bankservice \n", __func__);

    list_accounts(global_eid, &retval);
    return grpc::Status::OK;
}