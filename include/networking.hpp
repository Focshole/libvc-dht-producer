#include <assert.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <zmq.h>

#define CHUNK_SIZE 25000
namespace dht_prod
{
    int serveVersion(std::filesystem::path bin_path, int port);
    int serveVersion(std::filesystem::path bin_path, std::string socket);
}