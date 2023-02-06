# Simple bank app using SGX

## Install dependency

Ubuntu:
```
sudo apt install protobuf-compiler libgrpc++-dev protobuf-compiler-grpc
```

## Build

```
git clone https://gitlab.inf.telecom-sudparis.eu/pipereau/sgx-bank.git
cd sgx-bank
git submodule init
git submodule update
make
```

## Run

Server:
```
LD_LIBRARY_PATH=./build ./build/bank_server <bank_capacity>
```

Client:
```
./build/bank_client <nbr of accounts to create>
```
