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
    if(cache.contains(track.get_title()) == true) {
        cache.get(track.get_title());
        return 1;
    }

    else {

        PointerWrapper<AudioTrack> trackWrapper = track.clone();
        if(trackWrapper.get() == nullptr) 
        {
            throw std::runtime_error("[ERROR] Track " + track.get_title() +  "failed to clone and returning appropriate failure code without corrupting the cache state");
        }

        trackWrapper.get()->load();
        trackWrapper.get()->analyze_beatgrid();
        bool value = cache.put(std::move(trackWrapper));
        if(value == true) {
            return -1;
        }
        else 
            return 0;
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
    AudioTrack* ptr = cache.get(track_title);
    return ptr;
}