
#include <filesystem>
#ifndef OPENDHT_EXTRA_UTILS_HPP
#define OPENDHT_EXTRA_UTILS_HPP
#include <iostream>
#include <opendht.h>
#include <opendht/crypto.h>

#include <fstream>
#include <gnutls/gnutls.h>
#include <string>
#include <vector>
// if PR https://github.com/savoirfairelinux/opendht/pull/655 gets merged,
// replace all calls to importIdentity with dht::crypto::loadIdentity
namespace dht {
namespace crypto {
dht::crypto::Identity importIdentity(const std::string &path,
                                     const std::string &privkey_password);
}
} // namespace dht
#endif // OPENDHT_EXTRA_UTILS_HPP