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
