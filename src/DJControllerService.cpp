#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}

int DJControllerService::loadTrackToCache(AudioTrack &track)
{
    // Checking if the track already exist in the cache
    std::string track_title = track.get_title();
    if (cache.contains(track.get_title()))
    {
        cache.get(track_title);
        // Return 1 for a HIT
        return 1;
    }
    // If not, continue (MISS case)
    else
    {
        PointerWrapper<AudioTrack> track_clone = track.clone();
        // Checking if the cloning worked
        if (!track_clone)
        {
            std::cerr << "[ERROR] Track: \"" << track_title << "\" failed to clone" << std::endl;
            return 0;
        }
        // Unwrap the pointer to get the raw one
        AudioTrack *raw_pointer = track_clone.get();

        // Simulate load and analyze beatgrid
        raw_pointer->load();
        raw_pointer->analyze_beatgrid();

        // Using put function to move the cloned track into the cache and saving its return value in a boolean
        bool eviction_occurred = cache.put(std::move(track_clone));

        // Return -1 for a MISS with eviction and a 0 for a MISS wuthiut eviction
        if (eviction_occurred)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
}

void DJControllerService::set_cache_size(size_t new_size)
{
    cache.set_capacity(new_size);
}
// implemented
void DJControllerService::displayCacheStatus() const
{
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

AudioTrack *DJControllerService::getTrackFromCache(const std::string &track_title)
{
    AudioTrack *ptr = cache.get(track_title);
    return ptr;
}
