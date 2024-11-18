#ifndef PIECE_H
#define PIECE_H

#include <unordered_set>
#include <vector>
#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include "peer.h"
#include "torrent.h"

class Piece {
public:
    // Constructor
    Piece(size_t pieceIndex, const Torrent& torrent, const std::vector<Peer>& peers);

    // Getters
    const std::unordered_set<size_t>& getPeers() const;
    size_t getIndex() const;
    std::array<uint8_t, 20> getHash() const;
    size_t getLength() const;

    // Comparison operators for sorting
    bool operator<(const Piece& other) const;
    bool operator==(const Piece& other) const;

private:
    std::unordered_set<size_t> m_peers; // Indices of peers that have this piece
    size_t m_pieceIndex;
    size_t m_length;
    std::array<uint8_t, 20> m_hash;
};

#endif // PIECE_H
