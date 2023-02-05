#include "bank.grpc.pb.h"

class BankServiceImpl final : public bank::bank::Service
{
public:
    BankServiceImpl(std::string app) {}

    void BankCreate(int64_t cap);

    grpc::Status CreateAccount(grpc::ServerContext *context, const bank::AccountCreateReq *request, bank::AccountCreateResp *response) override;

    grpc::Status AddAmount(grpc::ServerContext *context, const bank::AmountAddReq *request, bank::AmountAddResp *response);

    grpc::Status SubAmount(grpc::ServerContext *context, const bank::AmountSubReq *request, bank::AmountSubResp *response);

    grpc::Status ListAccount(grpc::ServerContext *context, const bank::Empty *request, bank::Empty *response);

private:
    std::string name;
};
