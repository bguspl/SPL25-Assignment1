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
    for(const SessionConfig::TrackInfo& track : library_tracks) {
        AudioTrack* current;
        if(track.type == "MP3") {
        current = new MP3Track(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2);
        std::cout << "MP3Track created: " << track.extra_param1 << " kbps" << std::endl;
        }
        else if(track.type == "WAV") {
        current = new WAVTrack(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2);
        std::cout << "WAVTrack created: " << track.extra_param1 << " Hz" << std::endl << "/Bit depth: " << track.extra_param2 << std::endl;
        }
        library.push_back(current);
    }
    std::cout <<"[INFO] Track library built: " << library_tracks.size() << " tracks loaded" << std::endl;
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
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {

    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    Playlist new_playlist(playlist_name);
    int track_count = 0;
    for(int index : track_indices) {
        if( index > library.size()) {
            std::cout << "[WARNING] Invalid track index: " << index << std::endl;
            continue;
        }
        else{
            PointerWrapper<AudioTrack> audio_track(library[index - 1]->clone());
            if(!audio_track) {
                std::cout << "[WARNING] Failed to clone track: " << library[index - 1]->get_title() << std::endl;
                continue;
            }
            audio_track->load();
            audio_track->analyze_beatgrid();
            playlist.add_track(audio_track.release());
            track_count++;
            std::cout << "[INFO] Added " << library[index - 1]->get_title() << " to playlist" << playlist_name << std::endl;
        }
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << track_count << " tracks)" << std::endl;
}
/**
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> titles;
    for(const AudioTrack* track : playlist.getTracks()) {
        titles.push_back(track->get_title());
    }
    return titles;
}
