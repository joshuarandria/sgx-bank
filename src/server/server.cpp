#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>

#include <grpcpp/resource_quota.h>
#include "authbuilder.h"
#include "bankservice.h"

// #include "libbank/libbank.h"
// #include "hm_libbank/hm_libbank.h"

// Uncomment to use a single thread
//#define CONFIG_QUOTA

int main(int argc, char *argv[])
{
#ifdef CONFIG_QUOTA
	grpc::ResourceQuota quota;
	quota.SetMaxThreads(1);
#endif
	grpc::ServerBuilder builder;
#ifdef CONFIG_QUOTA
	builder.SetResourceQuota(quota);
	roc
#endif
		BankServiceImpl service("bank");
	std::string bind_addr = "localhost:50051";
	AuthBuilder auth;

	if (argc != 2)
		perror("usage: ./exec <capacity>\n");
	int capacity = atoi(argv[1]);

	auth.setInsecure(true);
	std::shared_ptr<grpc::ServerCredentials> cred = auth.build();

	builder.AddListeningPort(bind_addr, cred);
	builder.RegisterService(&service);

	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	// std::cout << "Using grpc " << grpc::Version() << std::endl;

	service.BankCreate(capacity);

	if (bind_addr.find(":") == std::string::npos)
	{
		std::cout << "Server listening on " << bind_addr << ":443" << std::endl;
	}
	else
	{
		std::cout << "Server listening on " << bind_addr << std::endl;
	}

	server->Wait();

	service.BankDestroy();
	std::cout << "Quit server" << std::endl;
}
