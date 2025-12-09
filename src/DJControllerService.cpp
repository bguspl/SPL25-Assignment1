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
    if(cache.contains(track.get_title())){
        cache.get(track.get_title());
        return 1;
    }
    else{
        PointerWrapper<AudioTrack> oldPtr = track.clone();
        AudioTrack* newPtr = oldPtr.get();
        if(newPtr == nullptr){
            throw std::runtime_error("null pointer exception");
        }
        newPtr->load();
        newPtr->analyze_beatgrid();
        if (cache.put(PointerWrapper<AudioTrack>(newPtr))){
            return -1;
        }

    }
    return 0; // Placeholder
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
    AudioTrack* ans = cache.get(track_title);
    return ans; // Placeholder
}
