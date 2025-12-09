#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : active_deck(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    auto_sync = false;
    bpm_tolerance = 0;
    std::cout << "Initializes with 2 empty decks..." << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    for(int i = 0; i < 2; i++){
        if(decks[i] != nullptr){
            delete decks[i];
            decks[i] = nullptr;
        }
    }
    std::cout << "Cleaning up decks..." << std::endl;
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here
    std::cout << "=== Loading track to Deck ===" << std::endl;
    PointerWrapper<AudioTrack> newAudio = track.clone();
    if(newAudio.get() == nullptr){
        std::cout << "[ERROR] Track:" << track.get_title() << "failed to clone" << std::endl;
        return -1;
    } 
    if(decks[0] == nullptr && decks[1] == nullptr){
        *decks[0] = *newAudio;
        active_deck = 0;
    }
    int targetDeck = 1 - active_deck;
    std::cout << "[Deck Switch] Target deck: " << targetDeck <<  std::endl;
    if(decks[targetDeck] != nullptr){
        delete decks[targetDeck];
        decks[targetDeck] = nullptr;
    }
    newAudio->load();
    newAudio->analyze_beatgrid();
    if(decks[0] != nullptr && decks[1] != nullptr && auto_sync){
        if(std::abs(newAudio->get_bpm() - decks[active_deck]->get_bpm()) >  bpm_tolerance){
            sync_bpm(newAudio);
        }
        decks[targetDeck] = newAudio.release();
        std::cout << "[Load Complete] " << decks[targetDeck]->get_title()  << " is now loaded on deck: " << targetDeck << std::endl;
    }
    if(decks[active_deck] != nullptr){
        std::cout << "[Unload] Unloading preevious deck " << decks[active_deck]->get_title()  <<  std::endl;
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }
    active_deck = targetDeck;
    std::cout << "[Active deck] Switched to deck " << targetDeck <<  std::endl;

    return targetDeck; // Placeholder
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
    if(decks[active_deck] == nullptr || track.get() == nullptr){
        return false;
    }
    if(std::abs(track->get_bpm() - decks[active_deck]->get_bpm()) <= bpm_tolerance){
        return true;
    }
    return false; // Placeholder
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(decks[active_deck] != nullptr && track.get() != nullptr){
        int origin = track->get_bpm();
        int avg = (origin + decks[active_deck]->get_bpm()) / 2;
        AudioTrack* toSet = track.get();
        toSet->set_bpm(avg);
        std::cout << "[Sync BPM] syncing BPM from " << decks[active_deck]->get_title() << " to " << track->get_title() <<  std::endl;
    }
}
