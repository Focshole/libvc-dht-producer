# LibVC DHT Producer

LibVersioningCompiler implementation to produce code for a specific device to run it. Uses Opendht and Zeromq to distribute the compiled code.

## Description

This is an example project to show how could be implemented 

## Build

It requires all [libVersioningCompiler dependencies](https://github.com/skeru/libVersioningCompiler#dependencies), plus [GNUtls](https://www.gnutls.org/), [zeromq (libzmq)](https://zeromq.org/get-started/?language=cpp) and [opendht](https://github.com/savoirfairelinux/opendht). Then you do

```bash
./install_libVC.sh && ./deploy.sh
```

## Usage

```bash
 ./build/libvc_dht_producer -h
 Usage: ./build/libvc_dht_producer [-hcpPs]
-c              Where to save/load private key and certificate. Specify it without ".pem" or ".crt"
-h              Print this help
-p              Private key certificate password to set/use. Default one is empty.
-P              Port to listen at for DHT traffic. Default one is 4224
-s              Bootstrap socket to connect to. If not specified, the bootstrap phase will be skipped
```
