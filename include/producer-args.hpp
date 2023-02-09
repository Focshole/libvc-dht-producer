#ifndef PRODUCER_ARGS_HPP
#define PRODUCER_ARGS_HPP
#include <filesystem>
#include <getopt.h>
#include <iostream>
#include <opendht.h>
#include <stdexcept>
#include <string>
#include <unistd.h>

struct params {
  const dht::crypto::Identity identity;
  const std::string dht_bootstrap_socket;
  const int dht_port;
};
params parseAndHandleParams(int argc, char **argv);
#endif // PRODUCER_ARGS_HPP