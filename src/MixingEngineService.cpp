#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0) 
{
    // Your implementation here
    decks[0] = nullptr;
    decks[1] = nullptr;
    std::cout <<"[MixingEngineService] Initialized with 2 empty decks."<< std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    // Your implementation here
    std::cout<<"[MixingEngineService] Cleaning up decks.... " << std::endl;
    delete decks[0];
    decks[0] = nullptr;
    delete decks[1];
    decks[1] = nullptr;
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here
    std::cout <<"=== Loading Track to Deck ==="<< std::endl;
    PointerWrapper<AudioTrack> toLoad = track.clone();
    if(!toLoad){ // Failed to clone
        std::cout <<"[ERROR] Track:\"";
        std::cout << track.get_title();
        std::cout <<"\"failed to clone" << std::endl;
        displayDeckStatus();
        return -1;
    }
    // First track operation
    // if(decks[0] == nullptr && decks[1] == nullptr){
    //     // Loading sim
    //     toLoad -> load(); 
    //     toLoad -> analyze_beatgrid();
    //     // Memory management 
    //     std::cout<<"[Load Complete]" << toLoad -> get_title();
    //     std::cout<<"is now loaded on deck 0"<<std::endl;
    //     decks[0] = toLoad.release();
    //     displayDeckStatus();
    //     return 1;
    // }

    int target = 1 - active_deck;

     // Syncing BPM if needed
    if(auto_sync && !can_mix_tracks(toLoad)){  // can_mix_tracks checks for nullptr in decks
            sync_bpm(toLoad);
        }

    // Deck is occupied and needs to be replaced
    if(decks[target]){
        std::cout << "[Deck Switch] Target deck:" <<target;
        std::cout<<"=== Clearing deck:"<<target<<"==="<<std::endl;
        // Memory management
        delete decks[target];
        decks[target] = nullptr;
    }
    
    // Loading sim
    toLoad -> load();
    toLoad -> analyze_beatgrid();
    std::cout<<"[Load Complete]" << toLoad -> get_title();
    std::cout<<"is now loaded on deck: " << target << std::endl;
    decks[target] = toLoad.release(); // Release handles PW's memory management
    
    active_deck = target;
    std::cout <<"Active deck is now" << target << std::endl;
    displayDeckStatus();
    return 1;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    // Nullptr check - cant mix
    if(!decks[active_deck] || !track.operator->()) return false;
    
    int activeBpm = decks[active_deck] -> get_bpm(), checkBpm = track -> get_bpm();
    if(abs(activeBpm - checkBpm) > bpm_tolerance) return false;
    return true;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(decks[active_deck] && track.operator->()){
        int toChange = track -> get_bpm(), activeBpm = decks[active_deck] -> get_bpm();
        int repBpm = ((toChange + activeBpm) / 2); // Getting the average bpm
        std::cout << "[Sync BPM] Synchin from" << toChange << "to" << repBpm << std::endl;
        track -> set_bpm(repBpm);
    }

}