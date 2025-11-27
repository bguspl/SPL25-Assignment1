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
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo> &library_tracks)
{
    for (const SessionConfig::TrackInfo track : library_tracks)
    {

        // If the track is an instance of mp3 :
        if (track.type == "MP3")
        {
            AudioTrack *new_track_mp3 = new MP3Track(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2); // Allocate a new mp3 track on heap
            library.push_back(new_track_mp3);                                                                                                                // Push to library
            std::cout << "MP3Track created: " << track.extra_param1 << "kbps" << std::endl;                                                                  // Log print
        }

        // If the track is an instance of WAV :
        else if (track.type == "WAV")
        {
            AudioTrack *new_track_wav = new WAVTrack(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2); // Allocate a new mp3 track on heap
            library.push_back(new_track_wav);                                                                                                                // Push to library
            std::cout << "WAVTrack created: " << track.extra_param1 << "Hz/" << track.extra_param2 << std::endl;                                             // Log print
        }
    }

    // Log summary
    std::cout << " [INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 *
 */
void DJLibraryService::displayLibrary() const
{
    std::cout << "=== DJ Library Playlist: "
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty())
    {
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
Playlist &DJLibraryService::getPlaylist()
{
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 *
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack *DJLibraryService::findTrack(const std::string &track_title)
{
    // Your implementation here
    return nullptr; // Placeholder
}

void DJLibraryService::loadPlaylistFromIndices(const std::string &playlist_name,
                                               const std::vector<int> &track_indices)
{
    // Your implementation here
    // For now, add a placeholder to fix the linker error
    (void)playlist_name; // Suppress unused parameter warning
    (void)track_indices; // Suppress unused parameter warning
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const
{
    // Your implementation here
    return std::vector<std::string>(); // Placeholder
}
