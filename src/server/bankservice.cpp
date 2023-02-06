#include "bankservice.h"

void BankServiceImpl::BankCreate(int64_t cap)
{
	printf("[%s] \n", __func__);
	bank = bank_create(cap);
}

void BankServiceImpl::BankDestroy()
{
	// bank_destroy(bank);
}

grpc::Status
BankServiceImpl::CreateAccount(grpc::ServerContext *context,
							   const bank::AccountCreateReq *request,
							   bank::AccountCreateResp *response)
{
	// std::cout << "Create account " << request->account_name() << std::endl;
	const char *account_name = request->account_name().c_str();
	account_create(bank, account_name);
	// account_create(request->account_name().c_str());
	response->set_new_amount(0);
	return grpc::Status::OK;
}

grpc::Status
BankServiceImpl::DeleteAccount(grpc::ServerContext *context,
							   const bank::AccountDeleteReq *request,
							   bank::AccountDeleteResp *response)
{
	std::cout << "Delete account " << request->account_name() << std::endl;
	const char *account_name = request->account_name().c_str();
	// account_delete(request->account_name().c_str());
	account_delete(bank, account_name);
	return grpc::Status::OK;
}

grpc::Status
BankServiceImpl::AddAmount(grpc::ServerContext *context,
						   const bank::AmountAddReq *request,
						   bank::AmountAddResp *response)
{
	const char *account_name = request->account_name().c_str();
	// int new_amount = amount_add(request->account_name().c_str(), request->amount());
	int new_amount = amount_add(bank, account_name, request->amount());
	response->set_new_amount(new_amount);

	return grpc::Status::OK;
}

grpc::Status
BankServiceImpl::SubAmount(grpc::ServerContext *context,
						   const bank::AmountSubReq *request,
						   bank::AmountSubResp *response)
{
	const char *account_name = request->account_name().c_str();

	// int new_amount = amount_sub(request->account_name().c_str(), request->amount());
	int new_amount = amount_sub(bank, account_name, request->amount());

	response->set_new_amount(new_amount);

	return grpc::Status::OK;
}

grpc::Status
BankServiceImpl::ListAccount(grpc::ServerContext *context,
							 const bank::Empty *request,
							 bank::Empty *response)
{

	list_accounts(bank);
	return grpc::Status::OK;
}
