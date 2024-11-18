#include "torrent.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "TinySHA1.hpp"

// Compute the info hash of the torrent
std::array<uint8_t, 20> Torrent::infoHash() const {
    std::ostringstream oss;
    oss << info.name << info.pieceLength;

    for (const auto& piece : info.pieces) {
        for (auto byte : piece) {
            oss << byte;
        }
    }

    sha1::SHA1 hasher;
    hasher.processBytes(oss.str().data(), oss.str().size());
    uint32_t digest[5];
    hasher.getDigest(digest);

    std::array<uint8_t, 20> hash;
    for (size_t i = 0; i < 5; ++i) {
        hash[i * 4 + 0] = static_cast<uint8_t>((digest[i] >> 24) & 0xFF);
        hash[i * 4 + 1] = static_cast<uint8_t>((digest[i] >> 16) & 0xFF);
        hash[i * 4 + 2] = static_cast<uint8_t>((digest[i] >> 8) & 0xFF);
        hash[i * 4 + 3] = static_cast<uint8_t>((digest[i] >> 0) & 0xFF);
    }

    return hash;
}

// Read and parse a `.torrent` file
std::shared_ptr<Torrent> Torrent::read(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open torrent file: " + filePath);
    }

    // Read file contents into a buffer
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());

    // Parse the buffer (simplified; replace with a real bencode parser)
    auto torrent = std::make_shared<Torrent>();
    torrent->announce = "http://tracker.example.com/announce"; // Example
    torrent->info.name = "example_file";
    torrent->info.pieceLength = 16384;
    torrent->info.pieces = {{/* Example hashes */}};
    torrent->info.type = TorrentType::SingleFile;
    torrent->info.length = 32768;

    return torrent;
}

// Print the file structure
void Torrent::printTree() const {
    if (info.type == TorrentType::SingleFile) {
        std::cout << info.name << std::endl;
    } else {
        for (const auto& file : info.files) {
            for (const auto& part : file.path) {
                std::cout << part << "/";
            }
            std::cout << std::endl;
        }
    }
}

// Get the total length of the torrent
size_t Torrent::length() const {
    if (info.type == TorrentType::SingleFile) {
        return info.length;
    } else {
        size_t totalLength = 0;
        for (const auto& file : info.files) {
            totalLength += file.length;
        }
        return totalLength;
    }
}

// Download all pieces (Placeholder function)
std::shared_ptr<Downloaded> Torrent::downloadAll() const {
    std::cout << "Downloading all pieces..." << std::endl;
    // Integrate with your download implementation
    return nullptr;
}
