#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "torrent.h"
#include "peer.h"
#include "piece.h"

struct DownloadedFile {
    File file;
    std::vector<uint8_t> bytes;

    std::string path() const;
};

class Downloaded {
public:
    std::vector<uint8_t> bytes; // Full downloaded data
    std::vector<File> files;    // File metadata

    class Iterator {
    public:
        Iterator(const Downloaded& downloaded, size_t offset = 0);

        bool hasNext() const;
        DownloadedFile next();

    private:
        const Downloaded& m_downloaded;
        size_t m_offset;
        size_t m_fileIndex;
    };

    Iterator iterator() const;
};

class Downloader {
public:
    static std::shared_ptr<Downloaded> downloadAll(const Torrent& torrent);

private:
    static void downloadPiece(const Piece& piece, const std::vector<Peer>& peers, std::vector<uint8_t>& allPieces);
    static std::vector<Peer> initializePeers(const Torrent& torrent, const std::array<uint8_t, 20>& infoHash);
};

#endif // DOWNLOAD_H
