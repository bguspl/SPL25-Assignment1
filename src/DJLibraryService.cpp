#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    //std::cout << "TODO: Implement DJLibraryService::buildLibrary method\n"<< library_tracks.size() << " tracks to be loaded into library.\n";
    for (SessionConfig::TrackInfo t : library_tracks) {
        if (t.type == "MP3") {
            library.push_back(new MP3Track(t.title, t.artists, t.duration_seconds, 
                                           t.bpm, t.extra_param1, t.extra_param2));
            std::cout << "MP3: MP3Track created: " << t.extra_param1 << " kbps" << std::endl;
        }
        else {
            library.push_back(new WAVTrack(t.title, t.artists, t.duration_seconds, 
                                           t.bpm, t.extra_param1, t.extra_param2));
            std::cout << "WAV: WAVTrack created: " << t.extra_param1 << "Hz/" << t.extra_param2 << "bit" << std::endl;
        }
    }
    std::cout << "[INFO] Track library built: " << library_tracks.size() << " tracks loaded" << std::endl;
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
    AudioTrack* track = playlist.find_track(track_title);
    return track;
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    playlist = Playlist(playlist_name);
    int tracks_count = 0;
    for (int i : track_indices) {
        if (i < 1 || i > library.size()) {
            std::cout << "[WARNING] Invalid track index: " << i << std::endl;
        }
        else {
            AudioTrack* track_original = library[i-1];
            AudioTrack* track_clone = track_original->clone().release();
            if (!track_clone) {
                std::cout << "[ERROR] Track: " << track_original->get_title() 
                          << " failed to clone." << std::endl;
            }
            else {
                track_clone->load();
                track_clone->analyze_beatgrid();
                playlist.add_track(track_clone);
                track_clone = nullptr;
                std::cout << "[INFO] Added '" << track_original->get_title() 
                          << "' to playlist '" << playlist_name << "'" << std::endl;
                tracks_count++;
            }
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" 
              << tracks_count << " tracks)" << std::endl;
    // For now, add a placeholder to fix the linker error
    (void)playlist_name;  // Suppress unused parameter warning
    (void)track_indices;  // Suppress unused parameter warning
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> titles;
    for (AudioTrack* t : playlist.getTracks()) {
        titles.push_back(t->get_title());
    }
    return titles;
}

DJLibraryService::~DJLibraryService() {
    for (AudioTrack* t : library) {
        delete t;
    }
    library.clear();
}
