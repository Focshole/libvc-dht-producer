#include "producer-utils.hpp"

namespace dht_prod
{
    std::shared_ptr<dht::DhtRunner> bootstrapDHTNode(const params &bootstrapParams)
    {
        auto node = std::make_shared<dht::DhtRunner>();
        node->run(bootstrapParams.dht_port, bootstrapParams.identity, true);
        if (bootstrapParams.dht_bootstrap_socket != "")
            node->bootstrap(bootstrapParams.dht_bootstrap_socket);
        return std::move(node);
    }

    std::filesystem::path publishVersion(std::shared_ptr<dht::DhtRunner> dhtNode, const vc::opt_list_t &compileOptions,
     std::filesystem::path sourceCode, std::string functionName, std::string dhtKey, std::string zmqPublicSocket,std::string zmqServingSocket)
    {
        auto bin_path = generateVersion(sourceCode, functionName, compileOptions);
        dhtNode->put(dhtKey,zmqPublicSocket.c_str());
        if (serveVersion(bin_path, zmqServingSocket)==0)
            return bin_path;
        else 
            return std::filesystem::path();
    }
}