#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    AudioTrack* toInsert = nullptr;
    int count = 0;
    for(size_t i = 0; i < library_tracks.size(); i++){
        auto curr = library_tracks[i];
        //Checking for type of object
        if(curr.type == "MP3"){
            // Creating appropriate object
            toInsert = new MP3Track(curr.title,curr.artists, curr.duration_seconds, curr.bpm, curr.extra_param1, curr.extra_param2);
            std::cout << "MP3Track created: " << curr.extra_param1 << " kbps" << std::endl;
            count++;
        }
        else{
            toInsert = new WAVTrack(curr.title,curr.artists, curr.duration_seconds, curr.bpm, curr.extra_param1, curr.extra_param2);
            std::cout << "WAVTrack created: " <<curr.extra_param1<< "/" <<curr.extra_param2<< std::endl;
            count++;
        }
        if(toInsert != nullptr){
            library.push_back(toInsert);
        }
    }
    std::cout<< "[INFO] Track library built: "<< count << " tracks loaded"<< std::endl;
}

DJLibraryService::~DJLibraryService() {
    for(AudioTrack* track : library) {
        delete track;
    }
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
    AudioTrack* found = playlist.find_track(track_title);
    if(found) return found;
    return nullptr;
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "[INFO] Loading playlist: " << playlist_name  << std::endl;
    playlist = Playlist(playlist_name);
    int len = library.size();
    int count = 0;
    for(int track_idx : track_indices) {
        int real_idx = track_idx - 1;
        if(real_idx < 0 || real_idx >= len) {
            std::cout << "[WARNING] Invalid track index: " << track_idx << std::endl;
        }
        else {
            AudioTrack* track_ptr = library[real_idx];
            PointerWrapper<AudioTrack> wrapped_clone = track_ptr->clone();
            if(!wrapped_clone) std::cout << "[ERROR] Null clone" << std::endl;
            else {
                wrapped_clone->load();
                wrapped_clone->analyze_beatgrid();
                AudioTrack* track_clone = wrapped_clone.release();
                playlist.add_track(track_clone);
                // std::cout << "Added '" << track_ptr->get_title()
                // << "' to playlist '" << playlist_name
                // << "'" << std::endl;
                count++;
            }

        }
    }
    std::cout << "[INFO] Playlist loaded: '" << playlist_name
    << "' (" << count << " tracks)" << std::endl;
}

/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    int len = playlist.get_track_count();
    std::vector<std::string> tracks_titles;
    for(int i = 0; i < len; i++) {
        tracks_titles.push_back(tracks[i]->get_title());
    }
    return tracks_titles; 
}
