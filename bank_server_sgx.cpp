#include <stdio.h>
#include <iostream>
#include "bank_server_sgx.h"
#include "sgx_utils.h"

#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>

#include <grpcpp/resource_quota.h>
#include "authbuilder.h"
#include "bankservice.h"
#include "Enclave_u.h"
#include "sgx_urts.h"

sgx_enclave_id_t global_eid = 0;
int retval;

// OCall implementations
void ocall_print(const char *str)
{
    printf("%s", str);
}

int main(int argc, char const *argv[])
{

    /*    ---------------  INIT ENCLAVE  ---------------   */

    if (initialize_enclave(&global_eid, "enclave.token", "enclave.signed.so") < 0)
    {
        std::cout << "Fail to initialize enclave." << std::endl;
        return 1;
    }
    std::cout << "Enclave initialized successfully!" << std::endl;

    /*    ---------------  INIT GRPC  ---------------   */

    BankServiceImpl service("bank");
    std::string bind_addr = "localhost:50051";
    AuthBuilder auth;
    grpc::ServerBuilder builder;

    /*    ---------------  BANK CREATE  ---------------   */

    auth.setInsecure(true);
    std::shared_ptr<grpc::ServerCredentials> cred = auth.build();
    builder.AddListeningPort(bind_addr, cred);
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    if (argc != 2)
        perror("usage = ./App_exec capacity");
    int capacity = atoi(argv[1]);

    service.BankCreate(capacity);

    if (retval == 1)
        std::cout
            << "Bank was created successfully! \n"
            << std::endl;
    else
        std::cout << "Failed creating bank." << std::endl;

    if (bind_addr.find(":") == std::string::npos)
    {
        std::cout << "Server listening on " << bind_addr << ":443" << std::endl;
    }
    else
    {
        std::cout << "Server listening on " << bind_addr << "\n\n"
                  << std::endl;
    }

    server->Wait();
    return 0;
}
