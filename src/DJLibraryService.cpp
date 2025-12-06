#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>

DJLibraryService::DJLibraryService(const Playlist &playlist)
    : playlist(playlist), library() {}

/**
 * Rule of 3 Implementation
 */

// Destructor
DJLibraryService::~DJLibraryService()
{
    // Delete all tracks in library vector
    for (AudioTrack *track : library)
    {
        delete track;
    }
}

// Copy constructor
DJLibraryService::DJLibraryService(const DJLibraryService &other)
    : playlist(other.playlist), library()
{
    // Deep copy of each track
    for (AudioTrack *track : other.library)
    {
        if (track != nullptr)
        {
            // Clone each track and store the raw pointer with release
            PointerWrapper<AudioTrack> cloned = track->clone();
            if (cloned)
            {
                library.push_back(cloned.release());
            }
        }
    }
}

// Copy assignment operator
DJLibraryService &DJLibraryService::operator=(const DJLibraryService &other)
{
    // Check if other eqaules to this
    if (this == &other)
    {
        return *this;
    }

    // Clean up existing library
    for (AudioTrack *track : library)
    {
        delete track;
    }

    // Deep copy the library vector
    for (AudioTrack *track : other.library)
    {
        if (track != nullptr)
        {
            PointerWrapper<AudioTrack> cloned = track->clone();
            if (cloned)
            {
                library.push_back(cloned.release());
            }
        }
    }

    // Copy playlist
    playlist = other.playlist;

    return *this;
}

// Move Constructor
DJLibraryService::DJLibraryService(DJLibraryService &&other) noexcept
    : playlist(std::move(other.playlist)), library(std::move(other.library))
{
    // We used std::move for both members
}

// Move Assignment Operator
DJLibraryService &DJLibraryService::operator=(DJLibraryService &&other) noexcept
{
    // Checking for equality
    if (this == &other)
    {
        return *this;
    }

    // Clean up current resources
    for (AudioTrack *track : library)
    {
        delete track;
    }

    // Using std::move to move libarary
    library = std::move(other.library);

    // Using std::move to move playlist
    playlist = std::move(other.playlist);

    return *this;
}

/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo> &library_tracks)
{
    for (const SessionConfig::TrackInfo &track : library_tracks)
    {

        // If the track is an instance of mp3 :
        if (track.type == "MP3")
        {
            AudioTrack *new_track_mp3 = new MP3Track(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2); // Allocate a new mp3 track on heap
            library.push_back(new_track_mp3);                                                                                                                // Push to library
        }

        // If the track is an instance of WAV :
        else if (track.type == "WAV")
        {
            AudioTrack *new_track_wav = new WAVTrack(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2); // Allocate a new mp3 track on heap
            library.push_back(new_track_wav);                                                                                                                // Push to library
        }
    }

    // Log summary
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;
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

AudioTrack *DJLibraryService::findTrack(const std::string &track_title)
{
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string &playlist_name,
                                               const std::vector<int> &track_indices)
{
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;

    // Create a new playlist (assign to member variable)
    playlist = Playlist(playlist_name);

    for (size_t i = 0; i < track_indices.size(); i++)
    {
        int index = track_indices[i];

        // Validate index (1-based, so valid range is 1 to library.size())
        if (index < 1 || index > static_cast<int>(library.size()))
        {
            std::cout << "[WARNING] Invalid track index: " << index << std::endl;
            continue; // Skip to next track
        }

        // Get track from library (convert 1-based to 0-based)
        AudioTrack *source_track = library[index - 1];

        // Clone track
        PointerWrapper<AudioTrack> track_clone = source_track->clone();

        // Check if cloning worked
        if (!track_clone)
        {
            std::cerr << "[ERROR] Track: \"" << source_track->get_title() << "\" failed to clone" << std::endl;
            continue; // Skip to next track
        }

        // Release ownership from wrapper
        AudioTrack *raw_pointer = track_clone.release();

        // Load and analyze
        raw_pointer->load();
        raw_pointer->analyze_beatgrid();

        // Add to playlist (playlist now owns the track)
        playlist.add_track(raw_pointer);
    }

    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << playlist.get_track_count() << " tracks)" << std::endl;
}

/**
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const
{

    // Allocating a new vector for the titles
    std::vector<std::string> track_titles;

    // Allocating a new memory for the AudioTracks pointers
    std::vector<AudioTrack *> tracks = playlist.getTracks();

    // Iterating through tracks to get titles from each one
    for (AudioTrack *track : tracks)
    {
        track_titles.push_back(track->get_title());
    }
    return track_titles;
}
