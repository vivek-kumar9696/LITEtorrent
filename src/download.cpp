#include <vector>
#include <string>
#include <unordered_set>
#include <queue>
#include <future>
#include <memory>
#include <map>
#include <algorithm>
#include <iostream>
#include <cassert>
#include "TinySHA1.hpp"
#include "torrent.h"
#include "peer.h"
#include "piece.h"
#include "tracker.h"

// Represents a downloaded file
struct File {
    size_t length;
    std::vector<std::string> path;
};

// Represents the final downloaded content
struct Downloaded {
    std::vector<uint8_t> bytes;
    std::vector<File> files;

    struct Iterator {
        const Downloaded* downloaded;
        size_t offset = 0;
        size_t fileIndex = 0;

        Iterator(const Downloaded* d) : downloaded(d), offset(0), fileIndex(0) {}

        bool hasNext() const {
            return fileIndex < downloaded->files.size();
        }

        std::pair<File, std::vector<uint8_t>> next() {
            const File& file = downloaded->files[fileIndex];
            size_t fileLength = file.length;

            std::vector<uint8_t> fileBytes(downloaded->bytes.begin() + offset,
                                           downloaded->bytes.begin() + offset + fileLength);

            offset += fileLength;
            fileIndex++;

            return {file, fileBytes};
        }
    };

    Iterator getIterator() const {
        return Iterator(this);
    }
};

Downloaded all(const Torrent& t) {
    auto infoHash = t.infoHash();
    auto peerInfo = TrackerResponse::query(t, infoHash);
    
    std::vector<std::shared_ptr<Peer>> peerList;

    for (const auto& peerAddr : peerInfo.peers) {
        try {
            auto peer = std::make_shared<Peer>(peerAddr, infoHash);
            peerList.push_back(peer);
            if (peerList.size() >= 5) {
                break;
            }
        } catch (const std::exception& e) {
            std::cerr << "Failed to connect to peer " << peerAddr << ": " << e.what() << std::endl;
        }
    }

    std::priority_queue<Piece> needPieces;
    std::vector<Piece> noPeers;

    for (size_t pieceIndex = 0; pieceIndex < t.info.pieces.size(); ++pieceIndex) {
        Piece piece(pieceIndex, t, peerList);
        if (piece.peers().empty()) {
            noPeers.push_back(piece);
        } else {
            needPieces.push(piece);
        }
    }

    // Ensure all pieces have peers
    assert(noPeers.empty());

    // Buffer to store downloaded pieces
    std::vector<uint8_t> allPieces(t.length(), 0);

    while (!needPieces.empty()) {
        auto piece = needPieces.top();
        needPieces.pop();

        size_t pieceSize = piece.length();
        size_t numBlocks = (pieceSize + (BLOCK_MAX - 1)) / BLOCK_MAX;

        std::vector<std::future<void>> tasks;
        std::vector<uint8_t> pieceData(pieceSize, 0);
        size_t bytesReceived = 0;

        for (auto& peer : peerList) {
            if (piece.peers().count(peer->id())) {
                tasks.emplace_back(std::async(std::launch::async, [&]() {
                    peer->downloadPiece(piece.index(), pieceData, bytesReceived);
                }));
            }
        }

        for (auto& task : tasks) {
            task.wait();
        }

        if (bytesReceived == pieceSize) {
            // Verify SHA1 hash
            SHA1 hasher;
            hasher.update(pieceData.data(), pieceData.size());
            std::string hash = hasher.final();

            if (hash != piece.hash()) {
                throw std::runtime_error("Piece hash verification failed");
            }

            std::copy(pieceData.begin(), pieceData.end(),
                      allPieces.begin() + piece.index() * t.info.pieceLength);
        } else {
            throw std::runtime_error("Failed to download piece: " + std::to_string(piece.index()));
        }
    }

    return Downloaded{
        .bytes = std::move(allPieces),
        .files = t.info.keys.singleFile ? std::vector<File>{{t.info.length, {t.info.name}}}
                                        : t.info.keys.multiFiles
    };
}
