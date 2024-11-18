#ifndef TRACKER_H
#define TRACKER_H

#include <vector>
#include <string>
#include "torrent.h"

struct TrackerResponse {
    struct PeerList {
        std::vector<std::string> peers;
    } peers;

    static TrackerResponse query(const Torrent& torrent, const std::string& infoHash);
};

#endif // TRACKER_H
