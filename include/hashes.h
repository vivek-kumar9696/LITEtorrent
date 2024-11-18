#ifndef HASHES_H
#define HASHES_H

#include <vector>
#include <array>
#include <cstdint>

class Hashes {
public:
    static std::vector<std::array<uint8_t, 20>> fromBytes(const std::vector<uint8_t>& bytes);
    static std::vector<uint8_t> toBytes(const std::vector<std::array<uint8_t, 20>>& hashes);
};

#endif // HASHES_H
