#ifndef PEER_H
#define PEER_H

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <memory>
#include <cstdint>
#include "message_framer.h"
#include "bitfield.h"
#include "torrent.h"

class Peer {
public:
    Peer(const asio::ip::tcp::endpoint& endpoint, const std::array<uint8_t, 20>& infoHash);

    bool hasPiece(size_t pieceIndex) const;

    void participate(size_t pieceIndex, size_t pieceSize, size_t numBlocks);

private:
    asio::ip::tcp::endpoint m_endpoint;
    asio::ip::tcp::socket m_socket;
    Bitfield m_bitfield;
    bool m_choked = true;

    void sendHandshake(const std::array<uint8_t, 20>& infoHash);
    void receiveHandshake();
    void sendInterested();
    void receiveUnchoke();
    void downloadPiece(size_t pieceIndex, size_t blockIndex, size_t blockSize);
};

#endif // PEER_H
