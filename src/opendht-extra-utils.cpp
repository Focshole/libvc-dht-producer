#ifndef OPENDHT_EXTRA_UTILS_CPP
#define OPENDHT_EXTRA_UTILS_CPP

#include "opendht-extra-utils.hpp"
namespace dht {
namespace crypto {
Identity importIdentity(const std::string &path,
                        const std::string &privkey_password = "") {
  std::ifstream pkStream(path + ".pem", std::ios::in | std::ios::binary);
  std::vector<uint8_t> pkContent((std::istreambuf_iterator<char>(pkStream)),
                                 std::istreambuf_iterator<char>());
  auto key = std::make_shared<PrivateKey>(pkContent, privkey_password);
  pkStream.close();
  // Create a certificate
  gnutls_x509_crt_t gnuCert;
  if (gnutls_x509_crt_init(&gnuCert) != GNUTLS_E_SUCCESS)
    throw std::runtime_error("Failed to initialize gnutls certificate struct");
  gnutls_datum_t crtContent;
  // Read the certificate file
  gnutls_load_file((path + ".crt").c_str(), &crtContent);
  gnutls_x509_crt_import(gnuCert, &crtContent, GNUTLS_X509_FMT_PEM);
  auto cert = std::make_shared<Certificate>(gnuCert);
  return {std::move(key), std::move(cert)};
}
} // namespace crypto
} // namespace dht

#endif // OPENDHT_EXTRA_UTILS_CPP