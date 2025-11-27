#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    if(cache.contains(track.get_title())) {
        cache.get(track.get_title());
        return 1;
    }

    PointerWrapper<AudioTrack> track_clone((track.clone()));
    if(!track_clone) {
        std::cout << "Failed to clone track: " << track.get_title() << std::endl;
        throw std::runtime_error("Failed to clone track");
    }

    track_clone->load();
    track_clone->analyze_beatgrid();
    PointerWrapper<AudioTrack> track_clone_ptr(std::move(track_clone));

    bool is_evicted = cache.put(std::move(track_clone_ptr));
    if(is_evicted) {
        return -1;
    }

    return 0;
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

AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    return cache.get(track_title);

}
