#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <iomanip>
#include "torrent.h"
#include "tracker.h"
#include "peer.h"
#include "piece.h"
#include "download.h"

void decodeBencodedValue(const std::string& encodedValue);
void displayTorrentInfo(const std::string& torrentPath);
void listPeers(const std::string& torrentPath);
void performHandshake(const std::string& torrentPath, const std::string& peerAddress);
void downloadPiece(const std::string& torrentPath, size_t pieceIndex, const std::string& outputPath);
void downloadTorrent(const std::string& torrentPath, const std::string& outputPath);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: bittorrent_client <command> [options]\n";
        return 1;
    }

    std::string command = argv[1];

    try {
        if (command == "decode" && argc == 3) {
            decodeBencodedValue(argv[2]);
        } else if (command == "info" && argc == 3) {
            displayTorrentInfo(argv[2]);
        } else if (command == "peers" && argc == 3) {
            listPeers(argv[2]);
        } else if (command == "handshake" && argc == 4) {
            performHandshake(argv[2], argv[3]);
        } else if (command == "download_piece" && argc == 5) {
            downloadPiece(argv[2], std::stoul(argv[3]), argv[4]);
        } else if (command == "download" && argc == 4) {
            downloadTorrent(argv[2], argv[3]);
        } else {
            std::cerr << "Invalid command or arguments.\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

void decodeBencodedValue(const std::string& encodedValue) {
    // Placeholder for decoding a bencoded value (not implemented here)
    std::cout << "Decoded Value: (Functionality not implemented in C++)\n";
}

void displayTorrentInfo(const std::string& torrentPath) {
    auto torrent = Torrent::read(torrentPath);

    std::cout << "Tracker URL: " << torrent->announce << "\n";
    std::cout << "Length: " << torrent->length() << "\n";
    std::cout << "Info Hash: ";
    for (auto byte : torrent->infoHash()) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    }
    std::cout << "\nPiece Length: " << torrent->info.pieceLength << "\n";
    std::cout << "Piece Hashes:\n";

    for (const auto& hash : torrent->info.pieces) {
        for (auto byte : hash) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
        }
        std::cout << "\n";
    }
}

void listPeers(const std::string& torrentPath) {
    auto torrent = Torrent::read(torrentPath);

    auto trackerResponse = TrackerResponse::query(*torrent, torrent->infoHash());
    for (const auto& peer : trackerResponse.peers) {
        std::cout << "Peer: " << peer.ip.to_string() << ":" << peer.port << "\n";
    }
}

void performHandshake(const std::string& torrentPath, const std::string& peerAddress) {
    auto torrent = Torrent::read(torrentPath);
    auto infoHash = torrent->infoHash();

    asio::ip::tcp::endpoint endpoint(asio::ip::make_address(peerAddress.substr(0, peerAddress.find(':'))),
                                     std::stoi(peerAddress.substr(peerAddress.find(':') + 1)));

    Peer peer(endpoint, infoHash);
    peer.connect();
    std::cout << "Handshake successful with peer at " << peerAddress << "\n";
}

void downloadPiece(const std::string& torrentPath, size_t pieceIndex, const std::string& outputPath) {
    auto torrent = Torrent::read(torrentPath);

    if (pieceIndex >= torrent->info.pieces.size()) {
        throw std::out_of_range("Invalid piece index.");
    }

    auto peers = Downloader::initializePeers(*torrent, torrent->infoHash());
    Piece piece(pieceIndex, *torrent, peers);

    std::vector<uint8_t> pieceData(torrent->info.pieceLength, 0);
    Downloader::downloadPiece(piece, peers, pieceData);

    std::ofstream outputFile(outputPath, std::ios::binary);
    outputFile.write(reinterpret_cast<const char*>(pieceData.data()), pieceData.size());
    std::cout << "Piece " << pieceIndex << " downloaded to " << outputPath << "\n";
}

void downloadTorrent(const std::string& torrentPath, const std::string& outputPath) {
    auto torrent = Torrent::read(torrentPath);

    auto downloaded = Downloader::downloadAll(*torrent);

    std::ofstream outputFile(outputPath, std::ios::binary);
    outputFile.write(reinterpret_cast<const char*>(downloaded->bytes.data()), downloaded->bytes.size());
    std::cout << "Torrent downloaded to " << outputPath << "\n";
}
