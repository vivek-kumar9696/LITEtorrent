#ifndef TORRENT_H
#define TORRENT_H

#include <string>
#include <vector>
#include <array>
#include <memory>

// Forward declaration for `Downloaded`
class Downloaded;

// Represents a file in a torrent (for multi-file torrents)
struct File {
    size_t length;             // Length of the file in bytes
    std::vector<std::string> path; // Path segments leading to the file
};

// Enum for keys in the Info section (SingleFile or MultiFile)
enum class TorrentType { SingleFile, MultiFile };

// Contains metadata from the `.torrent` file
struct TorrentInfo {
    std::string name;                     // Name of the file or directory
    size_t pieceLength;                   // Length of each piece
    std::vector<std::array<uint8_t, 20>> pieces; // SHA1 hashes of the pieces
    TorrentType type;                     // Type of the torrent
    size_t length;                        // Total length for single file
    std::vector<File> files;              // Files for multi-file torrents
};

// Represents a parsed `.torrent` file
class Torrent {
public:
    std::string announce; // URL of the tracker
    TorrentInfo info;     // Metadata from the `.torrent` file

    // Compute the info hash
    std::array<uint8_t, 20> infoHash() const;

    // Read and parse a `.torrent` file
    static std::shared_ptr<Torrent> read(const std::string& filePath);

    // Print the file structure
    void printTree() const;

    // Get the total length of the torrent
    size_t length() const;

    // Download all pieces (placeholder for integration with the `download` module)
    std::shared_ptr<Downloaded> downloadAll() const;
};

#endif // TORRENT_H
