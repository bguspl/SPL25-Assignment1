#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // Your implementation here
    if (cache.contains(track.get_title())) {    //HIT
        cache.get(track.get_title());
        return 1;
    }
    else {                          //MISS
        PointerWrapper<AudioTrack> pw = track.clone();
        AudioTrack* track_ptr = pw.release();
        if (!track_ptr) {
            std::cout << "[ERROR] Track: \""
                << track.get_title() << "\" failed to clone " << std::endl;
            return 0;
        }
        track_ptr->load();
        track_ptr->analyze_beatgrid();
        if (cache.put(PointerWrapper<AudioTrack>(track_ptr))) {
            return -1;               //with eviction
        }
        return 0;                    //no eviction
    }
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    // Your implementation here
    return cache.get(track_title);
}
