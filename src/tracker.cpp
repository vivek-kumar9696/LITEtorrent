#include "tracker.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cctype>
#include <sstream>
#include <curl/curl.h>

// Helper function to URL-encode
std::string urlencode(const std::array<uint8_t, 20>& infoHash) {
    std::ostringstream encoded;
    for (auto byte : infoHash) {
        encoded << '%' << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    }
    return encoded.str();
}

// Serialize `TrackerRequest` into URL parameters
std::string TrackerRequest::toUrlParams() const {
    std::ostringstream oss;
    oss << "peer_id=" << peer_id
        << "&port=" << port
        << "&uploaded=" << uploaded
        << "&downloaded=" << downloaded
        << "&left=" << left
        << "&compact=" << (int)compact;
    return oss.str();
}

// Helper function to parse peers from the tracker response
std::vector<PeerInfo> TrackerResponse::parsePeers(const std::string& peerData) {
    std::vector<PeerInfo> peers;

    if (peerData.size() % 6 != 0) {
        throw std::runtime_error("Invalid peers data length.");
    }

    for (size_t i = 0; i < peerData.size(); i += 6) {
        uint8_t ip1 = static_cast<uint8_t>(peerData[i]);
        uint8_t ip2 = static_cast<uint8_t>(peerData[i + 1]);
        uint8_t ip3 = static_cast<uint8_t>(peerData[i + 2]);
        uint8_t ip4 = static_cast<uint8_t>(peerData[i + 3]);
        uint16_t port = (static_cast<uint8_t>(peerData[i + 4]) << 8) |
                        static_cast<uint8_t>(peerData[i + 5]);

        peers.emplace_back(asio::ip::address_v4({ip1, ip2, ip3, ip4}), port);
    }

    return peers;
}

// Query the tracker and parse the response
TrackerResponse TrackerResponse::query(const Torrent& torrent, const std::array<uint8_t, 20>& infoHash) {
    // Build tracker URL
    TrackerRequest request{
        "00112233445566778899", // Example peer ID
        6881,                  // Example port
        0,                     // Uploaded
        0,                     // Downloaded
        torrent.length(),      // Left
        1                      // Compact format
    };

    std::string url = torrent.announce + "?" + request.toUrlParams() + "&info_hash=" + urlencode(infoHash);

    // Perform HTTP GET request
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize cURL.");
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](char* ptr, size_t size, size_t nmemb, std::string* data) {
        data->append(ptr, size * nmemb);
        return size * nmemb;
    });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error("Failed to query tracker: " + std::string(curl_easy_strerror(res)));
    }
    curl_easy_cleanup(curl);

    // Parse the response (simplified for compact format)
    TrackerResponse trackerResponse;
    trackerResponse.peers = parsePeers(response.substr(response.find("peers") + 6));
    return trackerResponse;
}
