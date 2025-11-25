#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : active_deck(0), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr; decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks..." << std::endl;
    for (int i = 0; i < 2; ++i) {
        if(decks[i]) {
            delete decks[i];
            decks[i] = nullptr;
        }
    }
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here
    std::cout << "\n=== Loading Track to Deck ===" << std::endl;            //a - log header

    PointerWrapper<AudioTrack> pw(track.clone());                           //b - clone track and wrap

    if (!pw) {                                                              //c - log error if clone fails
        std::cout << "[ERROR] Track: " << pw->get_title() << " failed to clone" << std::endl;
        return -1;
    }
    
    bool first_track = decks[0] == nullptr && decks[1] == nullptr;
    size_t target = first_track ? 0 : (1 - active_deck);                    //d - identify target deck

    std::cout << "[Deck Switch] Target deck: " << target << std::endl;      //e - log deck switch

    if (decks[target]) {                                                    //f - unload target if occupied
        delete decks[target];
        decks[target] = nullptr;
    }

    pw->load();                                                             //g - prepare cloned track
    pw->analyze_beatgrid();

    if (auto_sync                                                           //h - bpm management
            && decks[active_deck]   //if first, then decks[active_deck] is null
            && !can_mix_tracks(pw)) {
        sync_bpm(pw);
    }

    decks[target] = pw.release();                                           //i - release pw, assign, log
    std::cout << "[Load Complete] '" << decks[target]->get_title() << "' is now loaded on deck " << target << std::endl;
    
    if (!first_track) {                                                     //j - instant transition
        std::cout << "[Unload] Unloading previous deck " 
                  << active_deck << " (" << decks[active_deck]->get_title() << ")" << std::endl;
        delete decks[active_deck];
        decks[active_deck] = nullptr;
    }

    active_deck = target;                                                   //k - switch active deck
    
    std::cout << "[Active Deck] Switched to deck " << target << std::endl;  //l - log switch

    return target;  //casting                                               //m - return target index
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
    if (!decks[active_deck] || !track) {
        return false;
    }
    int active_bpm = decks[active_deck]->get_bpm();
    int new_bpm = track->get_bpm();
    int diff = active_bpm - new_bpm;
    if (diff < 0) { diff = -diff; }
    return diff <= bpm_tolerance;
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if (decks[active_deck] && track) {
        int active_bpm = decks[active_deck]->get_bpm();
        int original_bpm = track->get_bpm();
        int average = (active_bpm + original_bpm) / 2;
        track->set_bpm(average);
        std::cout << "[Sync BPM] Syncing BPM from " << original_bpm << " to " << average << std::endl;
    }
}
