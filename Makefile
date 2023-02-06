CXX=g++
CC=clang
LDFLAGS=`pkg-config --libs protobuf grpc++ grpc`\
	-Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed \
	-ldl -lpthread
CXXFLAGS=-std=c++11 -g
#CXXFLAGS=-Wall -Werror -O3 -std=c++11 -g
CFLAGS=-Wall -Werror -O3 -g

PROTOC=protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`


SRC=src
INC=./klib
BUILD=./build
PROTOS_PATH=proto

PROTO_OBJ=proto/bank.pb.o proto/bank.grpc.pb.o
#LIBBANK_OBJ=$(SRC)/server/libbank/libbank.o
LIBBANK_OBJ=$(SRC)/server/hm_libbank/hm_libbank.o $(SRC)/../hashmap/hashmap.o

SERV_OBJ=$(SRC)/server/server.o $(SRC)/server/authbuilder.o $(SRC)/server/bankservice.o
CLI_OBJ=$(SRC)/client/client.o $(SRC)/client/bankclient.o

all: bank_server bank_client

bank_server: $(PROTO_OBJ) $(SERV_OBJ) libbank.so
	$(info ****** Compile bank_server  ******)
	mkdir -p $(BUILD)
	$(CXX) $(PROTO_OBJ) $(SERV_OBJ) -L$(BUILD) -lbank $(CXXFLAGS) $(LDFLAGS) -o $(BUILD)/$@


bank_client: $(PROTO_OBJ) $(CLI_OBJ)
	$(info ****** Compile bank_client  ******)
	mkdir -p $(BUILD)
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $(BUILD)/$@


$(PROTO_OBJ): %.pb.o: %.pb.cc
	$(info ****** Compile protobuf generated CPP files ******)
	$(CXX) -c $(CXXFLAGS) $< -o $@

%.grpc.pb.cc: %.proto
	$(info ****** Compile proto to CPP GRPC *****)
	$(info $(GRPC_CPP_PLUGIN_PATH))
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=$(PROTOS_PATH) --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto
	$(info ****** Compile proto to CPP  ******)
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=$(PROTOS_PATH) $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I $(INC) -c -o $@ $<

libbank.so: $(LIBBANK_OBJ)
	$(info ****** Compile libbank  ******)
	mkdir -p $(BUILD)
	$(CC) $^ -shared -o $(BUILD)/$@

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -I $(INC) -c -o $@ $<

clean:
	rm -rf $(BUILD) $(PROTOS_PATH)/*.pb.cc $(PROTOS_PATH)/*.pb.h $(PROTOS_PATH)/*.pb.o $(SRC)/server/*.o $(SRC)/client/*.o $(SRC)/server/libbank/*.o $(SRC)/server/hm_libbank/*.o


test:
	#gcc -fPIC -shared testlib.c -o libtest
	#g++ test.c -o test -L . -l test
	gcc -E -c testlib.c -o libtest.o
	ar rcs libtest.a libtest.o
	g++ test.c -o test -L . -l test
