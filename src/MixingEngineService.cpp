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
     decks[0]=nullptr;
      decks[1]=nullptr;
      std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
  
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    // Your implementation here
    for(int i=0;i<2;i++)
    {
        delete decks[i];
        decks[i]=nullptr;
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) 
{
    // Your implementation here
    std::cout << "\n=== Loading Track to Deck ===" << std::endl;
    PointerWrapper<AudioTrack> track_clone=track.clone();
    if(!track_clone)
    {
          std::cout << "[ERROR] Track:" << track.get_title()<< " failed to clone" << std::endl;
          return -1;
    }
    int target_deck ;

    if( (decks[0] == nullptr && decks[1] == nullptr))  target_deck = 0;
    else target_deck = 1 - active_deck;

    std::cout << "[Deck Switch] Target deck: " << target_deck << std::endl;

    if(decks[target_deck]!=nullptr)
    {
        delete decks[target_deck];
        decks[target_deck]=nullptr; 
    }    
        track_clone->load();
        track_clone->analyze_beatgrid();

        if (decks[active_deck] != nullptr && auto_sync)
        { if (!can_mix_tracks(track_clone))sync_bpm(track_clone); }

        decks[target_deck] = track_clone.release();

    std::cout << "[Load Complete] '" << decks[target_deck]->get_title() 
              << "' is now loaded on deck " << target_deck << std::endl;

    if (decks[active_deck] != nullptr && active_deck != target_deck)
     {
        std::cout << "[Unload] Unloading previous deck " << active_deck 
                  << " (" << decks[active_deck]->get_title() << ")" << std::endl;
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }
    active_deck = target_deck;

    std::cout << "[Active Deck] Switched to deck " << target_deck << std::endl;

    return target_deck;
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
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const 
{
    // Your implementation here
    if(decks[active_deck]==nullptr || !track)
    return false; 
    if(std::abs(decks[active_deck]->get_bpm()-track->get_bpm())> bpm_tolerance)
    return false;

    return true;

}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const 
{
    // Your implementation here
    if(decks[active_deck]==nullptr || !track)return;
    int current_bpm = decks[active_deck]->get_bpm();
    int new_bpm = track->get_bpm();
    int average_bpm = (current_bpm + new_bpm) / 2;
    std::cout << "[Sync BPM] Syncing BPM from " << new_bpm << " to " << average_bpm << std::endl;
   

}
