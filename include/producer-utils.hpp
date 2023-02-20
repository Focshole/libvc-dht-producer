#include <filesystem>
#include <opendht.h>
#include "networking.hpp"
#include "versioningCompiler/Version.hpp"
namespace dht_prod
{
    struct params
    {
        const dht::crypto::Identity identity;
        const std::string dht_bootstrap_socket;
        const int dht_port;
    };
    // This function will create a dhtNode from the provided params. Only bootstrapParams.dht_bootstrap_socket is optional.
    std::shared_ptr<dht::DhtRunner> bootstrapDHTNode(const params &bootstrapParams);
    // this function compiles the object with the specified option flags, then publishes it on the dht with the key dhtKey and
    // serves the binary on the specified zmqServingSocket. Set the zmqPublicSocket to the publicly reachable socket. Returns the generated file path.
    std::filesystem::path publishVersion(std::shared_ptr<dht::DhtRunner> dhtNode,
                                         const vc::opt_list_t &compileOptions,
                                         std::filesystem::path sourceCode,
                                         std::string functionName,
                                         std::string dhtKey,
                                         std::string zmqPublicSocket,std::string zmqServingSocket);
}