#ifndef _BANK_SERVER_H
#define _BANK_SERVER_H

#include "../../proto/bank.grpc.pb.h"

#include "hm_libbank/hm_libbank.h"

class BankServiceImpl final : public bank::bank::Service
{
public:
	BankServiceImpl(std::string app) {}

	~BankServiceImpl()
	{
		std::cout << "Quitting Bank Server" << std::endl;
	}

	grpc::Status CreateAccount(grpc::ServerContext *context, const bank::AccountCreateReq *request, bank::AccountCreateResp *response) override;

	grpc::Status DeleteAccount(grpc::ServerContext *context, const bank::AccountDeleteReq *request, bank::AccountDeleteResp *response) override;

	grpc::Status AddAmount(grpc::ServerContext *context, const bank::AmountAddReq *request, bank::AmountAddResp *response) override;

	grpc::Status SubAmount(grpc::ServerContext *context, const bank::AmountSubReq *request, bank::AmountSubResp *response) override;

	grpc::Status ListAccount(grpc::ServerContext *context, const bank::Empty *request, bank::Empty *response) override;

	void BankCreate(int64_t cap);

	void BankDestroy();

private:
	std::string name;
	struct bank_ctx *bank;
};

#endif
