#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) {}

// =========================================================
// Rule of 3 Implementation for Playlist
// =========================================================
// Destructor
DJLibraryService::~DJLibraryService() {
    // Iterate over the vector and delete each track to prevent memory leaks
    for (AudioTrack* track : library) {
        delete track;
    }
    library.clear();
}

// Copy Constructor
DJLibraryService::DJLibraryService(const DJLibraryService& other) 
    : playlist(other.playlist) // Copy the playlist object
{
    // Deep copy the track library
    for (const AudioTrack* source_track : other.library) {
        if (source_track != nullptr) {
            // 1. Clone the track (polymorphic copy) - returns PointerWrapper
            // 2. Release ownership from wrapper to get the raw pointer
            // 3. Add to our new library vector
            library.push_back(source_track->clone().release());
        }
    }
}

// Copy Assignment Operator
DJLibraryService& DJLibraryService::operator=(const DJLibraryService& other) {
    // 1. Check for self-assignment
    if (this == &other) {
        return *this;
    }

    // 2. Clean up existing resources (same logic as destructor)
    for (AudioTrack* track : library) {
        delete track;
    }
    library.clear();

    // 3. Copy the playlist
    playlist = other.playlist;

    // 4. Deep copy the library (same logic as Copy Constructor)
    for (const AudioTrack* source_track : other.library) {
        if (source_track != nullptr) {
            // Clone and extract raw pointer
            library.push_back(source_track->clone().release());
        }
    }

    return *this;
}

/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    // Check the format field (“MP3” or “WAV”)
    for (size_t i = 0; i < library_tracks.size(); ++i) { //MP3
        if (library_tracks[i].type == "MP3" || library_tracks[i].type == "mp3") {
            // Create appropriate track type using the provided metadata
            AudioTrack* track = new MP3Track (library_tracks[i].title, library_tracks[i].artists, library_tracks[i].duration_seconds, library_tracks[i].bpm, library_tracks[i].extra_param1, library_tracks[i].extra_param2);
            // Store the raw pointer in the library vector
            library.push_back(track);
            // Log creation message
            std::cout << "[MP3Track created:] " << library_tracks[i].extra_param1 << " kbps" << std::endl;
        }
        else { //WAV
            // Create appropriate track type using the provided metadata
            AudioTrack* track = new WAVTrack (library_tracks[i].title, library_tracks[i].artists, library_tracks[i].duration_seconds, library_tracks[i].bpm, library_tracks[i].extra_param1, library_tracks[i].extra_param2);
            // Store the raw pointer in the library vector
            library.push_back(track);
            // Log creation message
            std::cout << "[WAVTrack created:] " << library_tracks[i].extra_param1 << "Hz/" << library_tracks[i].extra_param2 << "bit" << std::endl;
        }
    }
    // Log summary
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    // Log message
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    // Create new Playlist with the given name
    Playlist new_playlist(playlist_name);
    // For each index in the indices vector
    for (int raw_index : track_indices) {
        int real_index = raw_index - 1;
        // Validate index is within library bounds
        if (real_index < 0 || static_cast<size_t>(real_index) >= library.size()) {
            std::cout << "[WARNING] Invalid track index: " << raw_index << std::endl;
        }
        else {
            AudioTrack* track = library[real_index];
            if (track == nullptr) { //  If clone is nullptr, log error and skip
                std::cout << "[ERROR] Track is null" << std::endl;
            }
            else {
                // Clone the track polymorphically and unwrap the PointerWrapper
                AudioTrack* cloned_track = (track->clone()).release();
                // Call load() and analyze_beatgrid() on cloned track
                cloned_track->load();
                cloned_track->analyze_beatgrid();
                // Add cloned track to playlist
                new_playlist.add_track(cloned_track);
            }
        }
    }
    this->playlist = new_playlist;
    // Log message
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << new_playlist.get_track_count() << " tracks)" << std::endl;
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> track_titles;
    for (AudioTrack* track : playlist.getTracks()) {
        if (track != nullptr) track_titles.push_back(track->get_title());
    }
    return track_titles;
}