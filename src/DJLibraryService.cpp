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
    library = std::vector<AudioTrack*>(library_tracks.size());
    for(size_t i=0;i<library_tracks.size();i++){
        AudioTrack* track=nullptr;
        if(library_tracks[i].type=="MP3"){
            track=new MP3Track(library_tracks[i].title,library_tracks[i].artists,library_tracks[i].duration_seconds,
                                library_tracks[i].bpm, library_tracks[i].extra_param1, library_tracks[i].extra_param2);
            std::cout << "MP3Track created: " << library_tracks[i].extra_param1 << " kbps\n";
            library[i]=track;
        }
        else if(library_tracks[i].type=="WAV"){
            track=new WAVTrack(library_tracks[i].title,library_tracks[i].artists,library_tracks[i].duration_seconds,
                library_tracks[i].bpm, library_tracks[i].extra_param1, library_tracks[i].extra_param2);
            std::cout << "WAVTrack created: " << library_tracks[i].extra_param1 << "Hz/"<< library_tracks[i].extra_param2 << "bit\n";
            library[i]=track;
        }
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded\n";
}

    DJLibraryService::~DJLibraryService() { //destructor
        for (size_t i=0;i<library.size();i++)
            delete library[i];
        library.clear();
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
    // For now, add a placeholder to fix the linker error
    std::cout << "[INFO] Loading playlist: " << playlist_name << "\n";
    playlist = Playlist(playlist_name);
    int count=0;
    for(size_t i=0;i<track_indices.size();i++){
        int index=track_indices[i];
        if(index<1 || index>(int)library.size()){
            std::cout << "[WARNING] Invalid track index: " << index << "\n";
            continue;
        }
        AudioTrack* validTrack = library[index-1];
        PointerWrapper<AudioTrack> cl=validTrack->clone();
        if(!cl){
            std::cout << "[ERROR] Track: \""<<validTrack->get_title() << "\" failed to clone\n";
            continue;
        }
        cl->load();
        cl->analyze_beatgrid();
        playlist.add_track(cl.get());
        cl.release();
        std::cout<<"Added '"<<validTrack->get_title()<<"' to playlist '" << playlist_name <<"'\n";
        count++;
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << count << " tracks)\n";
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> titles;
    const std::vector<AudioTrack*>& tracks = playlist.getTracks();
    for(size_t i=0;i<tracks.size();i++)
        if(tracks[i]!=nullptr)
            titles.push_back(tracks[i]->get_title());
    return titles;
}
