#include <filesystem>
#include <iostream>
#include <opendht.h>
#include <string>
#include <vector>

#include "producer-args.hpp"
#include "libvc.hpp"
#include "networking.hpp"

const std::filesystem::path sourceCode =
    std::filesystem::u8path("../code-to-compile/example.cpp");
const std::string exampleFunctionName = "sha256";

int main(int argc, char **argv) {
  auto p = parseAndHandleParams(argc, argv);

  dht::DhtRunner node;
  // Launch a dht node on a new thread, using a
  // generated EC key pair, and listen on port 4224.
  // It is possible to use sockets too.
  node.run(p.dht_port, p.identity, true);
  std::cout << "DHT node started on port "<<p.dht_port << std::endl;

  // Join the network through any running node,
  // here using a known bootstrap node.
  // It is possible to use sockets too.
  if (p.dht_bootstrap_socket != "")
    node.bootstrap(p.dht_bootstrap_socket);

  // Generate the relevant code
  if (std::filesystem::exists(sourceCode)) {
    std::cout << "Generating binary from " << sourceCode << std::endl;
    // Options to compile and optimize the code
    const std::list<vc::Option> l = {
        vc::Option("openssl include", "-I", "/usr/include/openssl"),
        vc::Option("optimization for size flag", "-O", "s"),
        vc::Option("ssl lib", "-L", "/usr/lib/libcrypto.so"),
        vc::Option("-lcrypto", "-l", "crypto"),
        vc::Option("Position independent code", "-f", "PIC")};
    // generate the binary
    auto bin_path = generateVersion(sourceCode, exampleFunctionName, l);
    std::cout << "Generated binary at " << bin_path << std::endl;
    const unsigned int port = 4242;
    // Publish the socket address from which the binary can be downloaded
    node.putSigned(sourceCode, std::string("127.0.0.1:")+std::to_string(port));
    // Serve the binary forever
    auto ctx = serveVersion(bin_path, port);

  } else {
    std::cerr << "Source code file " << sourceCode << " does not exist"
              << std::endl;
    exit(-1);
  }
  node.join();
  return 0;
}
