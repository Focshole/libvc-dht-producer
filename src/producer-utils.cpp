#include "producer-utils.hpp"
#include "versioningCompiler/Utils.hpp" // don't include it globally or linking problems happen due to global variables
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
                                         std::filesystem::path sourceCode, std::string functionName, std::string dhtKey, std::string zmqPublicSocket, std::string zmqServingSocket)
    {
        vc::version_ptr_t version = vc::createVersion(sourceCode, functionName, compileOptions);
        dhtNode->put(dhtKey, zmqPublicSocket.c_str());
        if (serveVersion(version->getFileName_bin(), zmqServingSocket) == 0)
            return version->getFileName_bin();
        else
            return std::filesystem::path();
    }
}