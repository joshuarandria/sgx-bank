#include "bank.grpc.pb.h"

class BankClient
{
public:
    BankClient(std::shared_ptr<grpc::ChannelInterface> channel)
        : stub_(bank::bank::NewStub(channel)) {}

    float CreateAccount(std::string name);

    void AddAmount(std::string name, int64_t amount);

    void SubAmount(std::string name, int64_t amount);

    void ListAccount();

private:
    std::unique_ptr<bank::bank::Stub> stub_;
};
