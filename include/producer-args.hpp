#ifndef PRODUCER_ARGS_HPP
#define PRODUCER_ARGS_HPP
#include <filesystem>
#include <getopt.h>
#include <iostream>
#include <opendht.h>
#include <string>
#include <unistd.h>

struct params {
  dht::crypto::Identity identity;
  std::string bootstrap_socket;
};
params parseAndHandleParams(int argc, char **argv);
#endif // PRODUCER_ARGS_HPP