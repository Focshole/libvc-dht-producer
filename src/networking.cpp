#include "networking.hpp"

#define CHUNK_SIZE 250000
int serveVersion(std::filesystem::path bin_path, int port) {
  void *context = zmq_ctx_new();
  void *sock = zmq_socket(context, ZMQ_REP);
  std::string socket = "tcp://*:";
  socket += std::to_string(port);
  int rc = zmq_bind(sock, socket.c_str());
  assert(rc == 0);
  auto fileSize = std::filesystem::file_size(bin_path);
  std::ifstream bin(bin_path, std::ios::binary);

  while (true) {
    char buffer[2] = "\x00";
    zmq_recv(sock, buffer, 1, 0);
    if (buffer[0] == 'd') {
      auto remainingBytes = fileSize;
      auto sentBytes = 0;
      uint8_t buffer[CHUNK_SIZE];
      while (remainingBytes) {
        if (remainingBytes < CHUNK_SIZE) {
          bin.read((char *)buffer, remainingBytes);
          sentBytes = zmq_send(sock, buffer, remainingBytes, 0);
          remainingBytes = 0;
          bin.seekg(0); // rewind the file reader
        } else {
          bin.read((char *)buffer, CHUNK_SIZE);
          sentBytes = zmq_send(sock, buffer, CHUNK_SIZE, ZMQ_SNDMORE);
          remainingBytes -= CHUNK_SIZE;
        }
        if (sentBytes == -1) {
          std::cerr << "Error while sending file: " << zmq_strerror(zmq_errno())
                    << std::endl;
          remainingBytes = 0;
          bin.seekg(0); // rewind the file reader
        }
      }
    } else {
      std::cout << "Received improper request: " << buffer << std::endl;
    }
  }
  return 0;
}