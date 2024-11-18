#include "piece.h"

// Constructor
Piece::Piece(size_t pieceIndex, const Torrent& torrent, const std::vector<Peer>& peers)
    : m_pieceIndex(pieceIndex) {
    // Determine the piece hash
    m_hash = torrent.info.pieces[pieceIndex];

    // Determine the piece size
    if (pieceIndex == torrent.info.pieces.size() - 1) {
        size_t remainder = torrent.length() % torrent.info.pieceLength;
        m_length = (remainder == 0) ? torrent.info.pieceLength : remainder;
    } else {
        m_length = torrent.info.pieceLength;
    }

    // Collect peers that have this piece
    for (size_t i = 0; i < peers.size(); ++i) {
        if (peers[i].hasPiece(pieceIndex)) {
            m_peers.insert(i);
        }
    }
}

// Getters
const std::unordered_set<size_t>& Piece::getPeers() const {
    return m_peers;
}

size_t Piece::getIndex() const {
    return m_pieceIndex;
}

std::array<uint8_t, 20> Piece::getHash() const {
    return m_hash;
}

size_t Piece::getLength() const {
    return m_length;
}

// Comparison operators
bool Piece::operator<(const Piece& other) const {
    if (m_peers.size() != other.m_peers.size()) {
        return m_peers.size() < other.m_peers.size();
    }

    if (m_hash != other.m_hash) {
        return m_hash < other.m_hash;
    }

    if (m_length != other.m_length) {
        return m_length < other.m_length;
    }

    return m_pieceIndex < other.m_pieceIndex;
}

bool Piece::operator==(const Piece& other) const {
    return m_pieceIndex == other.m_pieceIndex &&
           m_length == other.m_length &&
           m_hash == other.m_hash &&
           m_peers == other.m_peers;
}
