#include "peer.h"
#include <iostream>
#include "TinySHA1.hpp"

Peer::Peer(const asio::ip::tcp::endpoint& endpoint, const std::array<uint8_t, 20>& infoHash)
    : m_endpoint(endpoint), m_socket(asio::io_context()) {
    // Connect to the peer
    asio::error_code ec;
    m_socket.connect(endpoint, ec);
    if (ec) {
        throw std::runtime_error("Failed to connect to peer: " + ec.message());
    }

    // Perform the handshake
    sendHandshake(infoHash);
    receiveHandshake();
}

void Peer::sendHandshake(const std::array<uint8_t, 20>& infoHash) {
    std::vector<uint8_t> handshake(68, 0);
    handshake[0] = 19; // Protocol name length
    std::string protocol = "BitTorrent protocol";
    std::copy(protocol.begin(), protocol.end(), handshake.begin() + 1);

    // Reserved bytes
    std::fill(handshake.begin() + 20, handshake.begin() + 28, 0);

    // Info hash
    std::copy(infoHash.begin(), infoHash.end(), handshake.begin() + 28);

    // Peer ID
    std::string peerId = "-PC0001-000000000000";
    std::copy(peerId.begin(), peerId.end(), handshake.begin() + 48);

    // Send handshake
    asio::write(m_socket, asio::buffer(handshake));
}

void Peer::receiveHandshake() {
    std::vector<uint8_t> handshake(68);
    asio::read(m_socket, asio::buffer(handshake));

    // Verify the protocol name
    std::string protocol(handshake.begin() + 1, handshake.begin() + 20);
    if (protocol != "BitTorrent protocol") {
        throw std::runtime_error("Invalid protocol name in handshake.");
    }

    // Verify reserved bytes (can be extended for custom extensions)
    // ...

    // Initialize bitfield (simplified for this example)
    m_bitfield = Bitfield::fromPayload({});
}

void Peer::sendInterested() {
    Message message{MessageTag::Interested, {}};
    MessageFramer framer;
    asio::write(m_socket, asio::buffer(framer.encode(message)));
}

void Peer::receiveUnchoke() {
    MessageFramer framer;
    std::vector<uint8_t> buffer(1024);
    asio::read(m_socket, asio::buffer(buffer));

    auto message = framer.decode(buffer);
    if (message.tag == MessageTag::Unchoke) {
        m_choked = false;
    } else {
        throw std::runtime_error("Expected unchoke message.");
    }
}

bool Peer::hasPiece(size_t pieceIndex) const {
    return m_bitfield.hasPiece(pieceIndex);
}

void Peer::participate(size_t pieceIndex, size_t pieceSize, size_t numBlocks) {
    if (!hasPiece(pieceIndex)) {
        throw std::runtime_error("Peer does not have the requested piece.");
    }

    sendInterested();
    receiveUnchoke();

    for (size_t blockIndex = 0; blockIndex < numBlocks; ++blockIndex) {
        size_t blockSize = (blockIndex == numBlocks - 1) ? pieceSize % BLOCK_MAX : BLOCK_MAX;

        downloadPiece(pieceIndex, blockIndex, blockSize);
    }
}

void Peer::downloadPiece(size_t pieceIndex, size_t blockIndex, size_t blockSize) {
    // Create request message
    Message request{
        MessageTag::Request,
        Request::create(pieceIndex, blockIndex * BLOCK_MAX, blockSize)
    };

    // Send request
    MessageFramer framer;
    asio::write(m_socket, asio::buffer(framer.encode(request)));

    // Receive the block
    std::vector<uint8_t> buffer(1024);
    asio::read(m_socket, asio::buffer(buffer));

    auto response = framer.decode(buffer);
    if (response.tag != MessageTag::Piece) {
        throw std::runtime_error("Expected piece message.");
    }

    // Process the block (save to file or buffer)
    // ...
}
