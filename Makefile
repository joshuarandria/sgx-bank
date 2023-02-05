all : bank.pb.o bank.grpc.pb.o enclave.signed.so bank_server_sgx bank_client

### ---------------GRPC--------------- ###

bank.grpc.pb.cc: bank.proto
	protoc --grpc_out=. --plugin=protoc-gen-grpc=/usr/bin/grpc_cpp_plugin $<

bank.pb.cc: bank.proto
	protoc --cpp_out=. $<

bank.pb.o: bank.pb.cc 
	gcc -c $< -o $@
	@echo " ==== bank.pb.o ==== OK "

bank.grpc.pb.o: bank.grpc.pb.cc
	gcc -c $< -o $@
	@echo " ==== bank.grpc.pb.o ==== OK "

### ---------------FLAG bank_server_sgx--------------- ###

SGX_SDK=/opt/intel/sgxsdk/
SGX_EDGER = $(SGX_SDK)/bin/x64/sgx_edger8r
bank_server_sgx_C_flag = -m64 -O2 -fPIC -Wno-attributes -I$(SGX_SDK)/include
bank_server_sgx_CPP_flag = $(bank_server_sgx_C_flag) -std=c++11 -g0
bank_server_sgx_CPP_files := bank_server_sgx.cpp sgx_utils.cpp bankservice.cpp authbuilder.cpp
bank_server_sgx_CPP_obj := $(bank_server_sgx_CPP_files:.cpp=.o)
LDFLAGS=`pkg-config --libs protobuf grpc++ grpc`\
	-Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed \
	-ldl -lpthread -lstdc++
bank_server_sgx_link_flag = $(LDFLAGS) -m64 -L$(SGX_SDK)/lib64 -lsgx_urts_sim -lpthread -lsgx_uae_service_sim

BUILD=./build


### ---------------Server--------------- ###

Enclave_u.c: $(SGX_EDGER) Enclave.edl
	$(SGX_EDGER) --untrusted Enclave.edl --search-path /opt/intel/sgxsdk/include

Enclave_u.o: Enclave_u.c
	gcc $(bank_server_sgx_C_flag) -c $< -o $@

bank_server_sgx: Enclave_u.o $(bank_server_sgx_CPP_obj) bank.pb.o bank.grpc.pb.o
	gcc $^ -o $@ $(bank_server_sgx_link_flag) $(LDFLAGS)
	@echo " ==== bank_server_sgxp ==== OK "

authbuilder.o :authbuilder.cpp 
	gcc $(bank_server_sgx_CPP_flag) $(LDFLAGS) -c $< -o $@


sgx_utils.o: sgx_utils.cpp
	gcc $(bank_server_sgx_CPP_flag) $(LDFLAGS) -c $< -o $@	


bankservice.o : bankservice.cpp 
	gcc $(bank_server_sgx_CPP_flag) $(LDFLAGS) -c $< -o $@

bank_server_sgx.o : bank_server_sgx.cpp 
	gcc $(bank_server_sgx_CPP_flag) $(LDFLAGS) -L$(BUILD) -c $< -o $@



### ---------------BANK--------------- ###
libbank.so: Bank.o
	mkdir -p $(BUILD)
	gcc $^ -I$(SGX_SDK)/include -shared -o $(BUILD)/$@
	@echo " ==== libbank.so ==== OK "


### ---------------ENCLAVE--------------- ###

#OPTION Enclave_Include_Paths = -IEnclave -I$(SGX_SDK)/include -I$(SGX_SDK)/include/tlibc -I$(SGX_SDK)/include/stlport
Enclave_Include_Paths = -I$(SGX_SDK)/include -I$(SGX_SDK)/include/tlibc -I$(SGX_SDK)/include/stlport
Enclave_C_flags = -m64 -nostdinc -fvisibility=hidden -fpie -fstack-protector $(Enclave_Include_Paths)
Enclave_link_flags = -m64 -O0 -g -Wl,--no-undefined -nostdlib -nodefaultlibs -nostartfiles -L$(SGX_SDK)/lib64 \
	-Wl,--whole-archive -lsgx_trts_sim -Wl,--no-whole-archive \
	-Wl,--start-group -lsgx_tstdc -lsgx_tcxx -lsgx_tcrypto -lsgx_tservice_sim -Wl,--end-group \
	-Wl,-Bstatic -Wl,-Bsymbolic -Wl,--no-undefined \
	-Wl,-pie,-eenclave_entry -Wl,--export-dynamic  \
	-Wl,--defsym,__ImageBase=0

Enclave_t.c: $(SGX_EDGER) Enclave.edl
	$(SGX_EDGER) --trusted Enclave.edl --search-path $(SGX_SDK)/include

Enclave_t.o: Enclave_t.c
	gcc $(Enclave_C_flags) -c $< -o $@

enclave.so: Enclave_t.o Enclave.o Bank.o 
	gcc $^ -o $@ $(Enclave_link_flags)
	@echo " ==== enclave.so ==== OK "

enclave.signed.so: enclave.so
	$(SGX_SDK)/bin/x64/sgx_sign sign -key Enclave_private.pem -enclave enclave.so -out $@ -config Enclave.config.xml
	@echo " ==== enclave.signed.so ==== OK "


Enclave.o : Enclave.cpp 
	gcc $(Enclave_C_flags) -std=c++03 -nostdinc++ -c $< -o $@ 

Bank.o: Bank.cpp
	gcc $(Enclave_C_flags) -std=c++03 -nostdinc++ -fPIC -c $< -o $@ 

### ---------------Client--------------- ###


Client_grpc.o : Client_grpc.cpp 
	gcc $(bank_server_sgx_CPP_flag) $(LDFLAGS) -lm -c $< -o $@	

Client.o : Client.cpp Client_grpc.cpp
	gcc $(bank_server_sgx_CPP_flag) $(LDFLAGS) -lm -c $< -o $@

bank_client: bank.pb.o bank.grpc.pb.o Client_grpc.o Client.o 
	mkdir -p $(BUILD)
	gcc $^ -std=c++11 -g $(LDFLAGS) -lm -o $(BUILD)/$@
	@echo " ==== bank_client ==== OK "



### --------------- CLEAN --------------- ###

clean:
	rm -rf ./*.pb.*
	rm -rf $(BUILD) 
	rm -rf enclave.* 
	rm -rf Bank.o
	rm -rf Enclave.o
	rm -rf Enclave_t* Enclave_u*
	rm -rf authbuilder.o
	rm -rf sgx_utils.o
	rm -rf bank_server_sgx.o
	rm -rf bankservice.o
	rm -rf bank_server_sgx
	rm -rf Client_grpc.o
	rm -rf Client.o
