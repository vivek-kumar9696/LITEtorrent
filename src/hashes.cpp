#include "hashes.h"
#include <stdexcept>

std::vector<std::array<uint8_t, 20>> Hashes::fromBytes(const std::vector<uint8_t>& bytes) {
    if (bytes.size() % 20 != 0) {
        throw std::invalid_argument("Invalid byte length for hashes.");
    }

    std::vector<std::array<uint8_t, 20>> hashes;
    for (size_t i = 0; i < bytes.size(); i += 20) {
        std::array<uint8_t, 20> hash;
        std::copy(bytes.begin() + i, bytes.begin() + i + 20, hash.begin());
        hashes.push_back(hash);
    }
    return hashes;
}

std::vector<uint8_t> Hashes::toBytes(const std::vector<std::array<uint8_t, 20>>& hashes) {
    std::vector<uint8_t> bytes;
    for (const auto& hash : hashes) {
        bytes.insert(bytes.end(), hash.begin(), hash.end());
    }
    return bytes;
}
