# LITEtorrent
A lighter version of bittorrent- made as fun project for my research lab to distribute huge files among all the computers by uploading into one. This repository contains a fully functional BitTorrent client implemented in C++. It allows users to parse `.torrent` files, query peers, perform handshakes, and download files via the BitTorrent protocol.

---

## **Features**

- Display torrent metadata (tracker URL, file size, piece hashes, etc.).
- Query peers from the tracker.
- Perform handshakes with peers.
- Download specific pieces or entire torrents.
- Verify file integrity using SHA1 hashes.

---

### **Prerequisites**

- **Compiler**: `g++` supporting C++17.
- **Libraries**: 
  - `libcurl` (for HTTP requests).
  - `pthread` (for threading).
  - `TinySHA1`(for SHA1 hashing)
- **Tools**: `make`.

### **Building**

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/bittorrent-client.git
    cd bittorrent-client
    ```

2. Building the Project
    ```bash
    make
    ```
    This generates
    1. Debug build: Located at build/debug/bittorrent_client with symbols for GDB and ThreadSanitizer.
    2. Release build: Located at build/release/bittorrent_client optimized for performance.

3. Clean build files
    ```bash
    make clean
    ```

## **Usage**

### **General Syntax**
```bash
bittorrent_client <command> [options]
```

| Command                  | Description                                                          |
|--------------------------|----------------------------------------------------------------------|
| `info <torrent>`         | Display torrent metadata (tracker URL, file size, piece hashes, etc.). |
| `peers <torrent>`        | List available peers for the torrent.                               |
| `handshake <torrent> <peer>` | Test handshake with a specific peer.                               |
| `download_piece <torrent> <piece_index> <output>` | Download a specific piece to a file.                      |
| `download <torrent> <output>` | Download the entire torrent to a specified file.                  |

## **Remaining Features**

1. **Directory Downloads**:
   - Implement support for multi-file torrents to download entire directories.

2. **Seeding**:
   - Add functionality to allow the client to seed downloaded files to other peers.

3. **Resumable Downloads**:
   - Save the download progress to disk to enable resuming interrupted downloads.

4. **Advanced Peer Selection**:
   - Implement smarter peer selection algorithms based on factors like latency, bandwidth, and reliability.

5. **Error Handling**:
   - Enhance the handling of:
     - Connection drops.
     - Peer timeouts.
     - Tracker response errors.

6. **Improved UI and Logs**:
   - Add more user-friendly output with progress indicators and detailed logs.

7. **Parallel Piece Downloading**:
   - Optimize the client to download multiple pieces in parallel from different peers.

8. **Encrypted Communication**:
   - Add support for encrypted peer-to-peer communication for increased security.

9. **Tracker Protocol Extensions**:
   - Support additional tracker protocols such as UDP tracker requests.

10. **Cross-Platform Support**:
    - Ensure compatibility across different operating systems (Linux, macOS, Windows).

## **Acknowledgements**

This project was made possible thanks to the contributions of the following resources and inspirations:

1. [BitTorrent Protocol Documentation](https://www.bittorrent.org/beps/bep_0003.html) - For detailed specifications and understanding of the BitTorrent protocol.
2. [TinySHA1 Library](https://github.com/mohaps/TinySHA1) - Used for SHA1 hashing in this project.
3. [Arpit Bhayani-BitTorrent Internals] (https://www.youtube.com/watch?v=v7cR0ZolaUA&list=PLsdq-3Z1EPT1rNeq2GXpnivaWINnOaCd0) - Great resource to learn about bittorrent internals
4. (https://app.codecrafters.io/courses/bittorrent/introduction)

We are grateful for the work of these authors and communities that supported the development of this project.

