#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include "PointerWrapper.h"

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    if(cache.contains(track.get_title())){ // Check for existance 
        cache.get(track.get_title());
        return 1;
    }
    else{
        PointerWrapper<AudioTrack> clone = track.clone();
        if(!clone){
            std::cerr << "[ERROR] Failed to clone track: " << track.get_title() << std::endl;
            return 0; // MISS
        } 
        else{
            // Cloning the new track, simulating as required and wrapping it again
            AudioTrack* clonedTrack = clone.release();
            clonedTrack -> load();
            clonedTrack -> analyze_beatgrid();
            PointerWrapper<AudioTrack> clone2(clonedTrack);
            bool evict = false;
            evict = cache.put(std::move(clone2));
            cache.displayStatus();
            if(evict) return -1;
            else return 0;
        }
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
    AudioTrack* track = cache.get(track_title);
    if(!track) { // the track is null
        std::cout << "[ERROR] Track: <" << track_title << "> failed to clone" << std::endl;
        return nullptr;
    }
    return track;
}