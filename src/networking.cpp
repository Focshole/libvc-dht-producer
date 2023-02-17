#include "networking.hpp"

namespace dht_prod
{
  int serveVersion(std::filesystem::path bin_path, int port)
  {
    std::string socket = "tcp://*:";
    socket += std::to_string(port);
    return serveVersion(bin_path, socket);
  }

  int serveVersion(std::filesystem::path bin_path, std::string socket)
  {
    void *context = zmq_ctx_new(); // Create a throwaway context that gets destroyed once a version had been served
    auto fileSize = std::filesystem::file_size(bin_path);
    std::ifstream bin(bin_path, std::ios::binary);
    void *sock = zmq_socket(context, ZMQ_PUSH);
    int rc = zmq_bind(sock, socket.c_str());
    assert(rc == 0);
    auto remainingBytes = fileSize;
    auto sentBytes = 0;
    uint8_t buffer[CHUNK_SIZE];
    while (remainingBytes)
    {
      if (remainingBytes < CHUNK_SIZE)
      {
        bin.read((char *)buffer, remainingBytes);
        sentBytes = zmq_send(sock, buffer, remainingBytes, 0);
        remainingBytes = 0;
      }
      else
      {
        bin.read((char *)buffer, CHUNK_SIZE);
        sentBytes = zmq_send(sock, buffer, CHUNK_SIZE, 0);
        remainingBytes -= CHUNK_SIZE;
      }
      if (sentBytes == -1)
      {
        std::cerr << "Error while sending file: " << zmq_strerror(zmq_errno())
                  << std::endl;
        remainingBytes = 0;
        zmq_close(sock);
        zmq_ctx_destroy(context);
        bin.close();
        return -1;
      }
    }
    zmq_close(sock);
    zmq_ctx_destroy(context);
    bin.close();
    return 0;
  }
}