#include "bankservice.h"
#include "Client_grpc.h"
#include <time.h>
#include <math.h>

struct timespec start_time, end_time;

float BankClient::CreateAccount(std::string name)
{
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    grpc::ClientContext context;
    bank::AccountCreateReq request;
    bank::AccountCreateResp reply;
    request.set_account_name(name);

    grpc::Status status = stub_->CreateAccount(&context, request, &reply);
    if (status.ok())
    {
        // std::cout << "new_amount on account " << name << ": " << reply.new_amount() << std::endl;
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        long long int elapsed_ns = (end_time.tv_sec - start_time.tv_sec) * pow(10, 9) + (end_time.tv_nsec - start_time.tv_nsec);
        return (elapsed_ns / pow(10, 9));
    }
    else
    {
        std::cerr << status.error_code() << ": " << status.error_message() << std::endl;
        return 0;
    }
}

void BankClient::AddAmount(std::string name, int64_t amount)
{
    grpc::ClientContext context;
    bank::AmountAddReq request;
    bank::AmountAddResp reply;

    // clock_gettime(CLOCK_MONOTONIC, &start_time);
    request.set_account_name(name);
    request.set_amount(amount);

    // std::cout << "AddAmount " << request.amount() << " on account " << name << " ; " << std::endl;

    grpc::Status status = stub_->AddAmount(&context, request, &reply);
    if (status.ok())
    {
        // std::cout << "New amount is " << reply.new_amount() << std::endl;
    }
    else
    {
        std::cerr << status.error_code() << ": " << status.error_message() << std::endl;
    }
}

void BankClient::SubAmount(std::string name, int64_t amount)
{
    grpc::ClientContext context;
    bank::AmountSubReq request;
    bank::AmountSubResp reply;

    request.set_account_name(name);
    request.set_amount(amount);
    // std::cout << "SubAmount " << request.amount() << " on account " << name << " ; " << std::endl;

    grpc::Status status = stub_->SubAmount(&context, request, &reply);
    if (status.ok())
    {
        // std::cout << "New amount on account " << name << " is " << reply.new_amount() << std::endl;
    }
    else
    {
        std::cerr << status.error_code() << ": " << status.error_message() << std::endl;
    }
}

void BankClient::ListAccount()
{
    grpc::ClientContext context;
    bank::Empty request;
    bank::Empty reply;

    grpc::Status status = stub_->ListAccount(&context, request, &reply);
    if (status.ok())
    {
        std::cout << "list ok" << std::endl;
    }
    else
        std::cerr << status.error_code() << ": " << status.error_message() << std::endl;
}