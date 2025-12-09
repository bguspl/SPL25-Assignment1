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
    for(SessionConfig::TrackInfo element : library_tracks){
        if(element.type == "MP3"){
            MP3Track* newTrack = new MP3Track(element.title, element.artists, element.duration_seconds,
            element.bpm, element.extra_param1, element.extra_param2);
            if(newTrack != nullptr){
                this->library.push_back(newTrack);
                std::cout << "MP3Track created: " << newTrack->get_bitrate() << " kbps" << std::endl;
            }
        }
        else if(element.type == "WAV"){
            WAVTrack* newTrack = new WAVTrack(element.title, element.artists, element.duration_seconds,
            element.bpm, element.extra_param1, element.extra_param2);
            if(newTrack != nullptr){
                this->library.push_back(newTrack);
                std::cout << "WAVTrack created: " << newTrack->get_sample_rate() << " Hz/" << newTrack->get_bit_depth()<< " bit" << std::endl;
            }
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
    return playlist.find_track(track_title); // Placeholder
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    Playlist* myplaylist = new Playlist(playlist_name);
    for(int element : track_indices){
        if(element > library.size()){
            std::cout << "[WARNING] Invalid track index: "<< element << std::endl;
        }
        else{
            PointerWrapper<AudioTrack> toInsert = library.at(element - 1)->clone();
            AudioTrack* insert = toInsert.release();
            if(insert == nullptr){
                std::cout << "[ERROR] nullpointer exception you mother fuckers" << std::endl;
            }
            else{
                insert->load();
                insert->analyze_beatgrid();
                myplaylist->add_track(insert);
                std::cout << "Added " << insert->get_title() << " to playlist " << playlist_name << std::endl;
            }
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << myplaylist->get_track_count() << " tracks)" << std::endl;
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
    std::vector<AudioTrack*> toCheck = playlist.getTracks();
    std::vector<std::string> checked;
    for(AudioTrack* element : toCheck){
        checked.push_back(element->get_title());
    }
    return checked; // Placeholder
}
