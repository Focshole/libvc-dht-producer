#ifndef PRODUCER_ARGS_CPP
#define PRODUCER_ARGS_CPP
#include "producer-args.hpp"
#include "opendht-extra-utils.hpp"

static const constexpr struct option args_getopt_struct[] = {
    {"help", no_argument, nullptr, 'h'},
    {"priv_cert", optional_argument, nullptr, 'c'},
    {"priv_cert_passw", optional_argument, nullptr, 'p'},
    {"dht_port", optional_argument, nullptr, 'P'},
    {"dht_bootstrap_socket", optional_argument, nullptr, 's'},
    {nullptr, 0, nullptr, 0}};

struct args {
  bool help{false};
  std::filesystem::path priv_cert{""};
  std::string priv_cert_passw{""};
  std::string dht_bootstrap_socket{""};
  int dht_port{4224};
};

void printUsage(const char *filename) {
  std::cout << "Usage: " << filename << " [-h] [-cpPs args]\n"
            << "-c\t\tWhere to save/load private key and certificate. Specify "
               "the path without \".pem\" or \".crt\"\n"
            << "-h\t\tPrint this help\n"
            << "-p\t\tPrivate key certificate password to set/use. Default one "
               "is empty.\n"
            << "-P\t\tPort to listen at for DHT traffic. Default one is 4224\n"
            << "-s\t\tBootstrap socket to connect to. If not specified, the "
               "bootstrap phase will be skipped"
            << std::endl;
}

args parseArgs(int argc, char **argv) {
  args params;
  int opt;
  while ((opt = getopt_long(argc, argv, "hc:p:P:s:", args_getopt_struct,
                            nullptr)) != -1) {
    switch (opt) {
    case 'c':
      params.priv_cert = optarg;
      break;
    case 'h':
      printUsage(argv[0]);
      params.help = true;
      exit(0);
    case 'p':
      params.priv_cert_passw = optarg;
      break;
    case 'P':
      try {
        params.dht_port = atoi(optarg);
        if (params.dht_port <= 0 || params.dht_port > 65535) {
          throw std::out_of_range("Port number out of range");
        }
      } catch (std::invalid_argument &e) {
        std::cerr << "Invalid port number value" << std::endl;
        printUsage(argv[0]);
        exit(-1);
      } catch (std::out_of_range &e) {

        std::cerr << "Invalid port number value: out of range. Using default "
                     "port 4224."
                  << std::endl;
        params.dht_port = 4224;
      }
      break;
    case 's':
      params.dht_bootstrap_socket = optarg;
      break;
    default:
      printUsage(argv[0]);
      exit(-1);
    }
  }
  return params;
}

params parseAndHandleParams(int argc, char **argv) {
  auto params = parseArgs(argc, argv);
  if (params.priv_cert == "") {
    params.priv_cert = "dhtCert";
  }

  if (!std::filesystem::exists(params.priv_cert.string() + ".pem") &&
      !std::filesystem::exists(params.priv_cert.string() + ".crt")) {
    dht::crypto::Identity id = dht::crypto::generateEcIdentity("producerNode");
    dht::crypto::saveIdentity(id, params.priv_cert.string(),
                              params.priv_cert_passw); // save to file
    return {id, params.dht_bootstrap_socket, params.dht_port};
  } else if (std::filesystem::exists(params.priv_cert.string() + ".pem") &&
             std::filesystem::exists(params.priv_cert.string() + ".crt")) {
    std::cout << "Importing private key from " << params.priv_cert.c_str()
              << ".pem and "
              << "certificate from " << params.priv_cert.c_str() << ".crt ..."
              << std::endl;
    return {dht::crypto::importIdentity(params.priv_cert.string(),
                                        params.priv_cert_passw),
            params.dht_bootstrap_socket, params.dht_port};
  } else {
    std::cerr << "Expecting both private key file" << params.priv_cert
              << ".pem and certificate file " << params.priv_cert
              << ".crt or none of those to generate a new one.\n I had found "
                 "only the "
              << ((std::filesystem::exists(params.priv_cert.string() + ".pem"))
                      ? "key file\n"
                      : "certificate\n");
    exit(-1);
  }
}
#endif // PRODUCER_ARGS_CPP