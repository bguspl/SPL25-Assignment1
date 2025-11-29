#include "MixingEngineService.h"
#include <iostream>
#include <memory>


MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr; decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
}

MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks...." << std::endl;
    if(decks[0]) {
        delete decks[0];
        decks[0] = nullptr;
    }
    if(decks[1]) {
        delete decks[1];
        decks[1] = nullptr;
    }
}

/**
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "\n=== Loading Track to Deck ===";
    // Your implementation here
    PointerWrapper<AudioTrack> track_clone(track.clone());
    if(!track_clone) {
        std::cout << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone" << std::endl;
        return -1;
    }
    int target_deck = 1 - active_deck;
    std::cout << "[Deck Switch] Target deck: " << target_deck << std::endl;
    if(decks[target_deck]){
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }
    track_clone->load();
    track_clone->analyze_beatgrid();

    if(auto_sync && decks[active_deck]) {
        int bpm_diff = decks[active_deck]->get_bpm() - track_clone->get_bpm();
        if(bpm_diff > bpm_tolerance) {
            sync_bpm(track_clone); //sync_bpm is implemented in the next section
        }  
    }
    decks[target_deck] = track_clone.release();
    std::cout << "[Load Complete] \"" << track.get_title() << "\" is now loaded on deck " << target_deck << std::endl;
    if(decks[active_deck]){
        std::cout << "[Unload] Unloading previous deck " << active_deck << " (" << decks[active_deck]->get_title() << ")" << std::endl;
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
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
if(!decks[active_deck] || !track) {
    return false; 
}
int bpm_diff = decks[active_deck]->get_bpm() - track->get_bpm();
if(std::abs(bpm_diff) <= bpm_tolerance) {
    return true;
}
return false;
}
/**
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if(!decks[active_deck] || !track) {
        return; 
    }
    int track_bpm = track->get_bpm();
    int active_deck_bpm = decks[active_deck]->get_bpm();
    int new_bpm = (track_bpm + active_deck_bpm) / 2;
    track->set_bpm(new_bpm);
    std::cout << "[BPM Sync] Syncing BPM from " << track->get_title() << " to " << new_bpm << std::endl;
}
