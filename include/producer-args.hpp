#ifndef PRODUCER_ARGS_HPP
#define PRODUCER_ARGS_HPP
#include <filesystem>
#include <getopt.h>
#include <iostream>
#include <opendht.h>
#include <stdexcept>
#include <string>
#include <unistd.h>

#include "opendht-extra-utils.hpp"
#include "producer-utils.hpp"

dht_prod::params parseAndHandleParams(int argc, char **argv);
#endif // PRODUCER_ARGS_HPP