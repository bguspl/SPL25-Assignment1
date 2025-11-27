#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    auto_sync = false;
    bpm_tolerance = 0;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks..." << std::endl;
    for (int i = 0; i < 2; ++i) {
        if (decks[i] != nullptr) {
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
    int target_deck;
    bool first_track = false;

    std::cout << "\n=== Loading Track to Deck ===" << std::endl;

    PointerWrapper<AudioTrack> clonetrack = track.clone();
    if (clonetrack.get() == nullptr) {
        std::cerr << "[ERROR] Track: \"" << track.get_title()
                  << "\" failed to clone" << std::endl;
        return -1;
    }

    if (decks[0] == nullptr && decks[1] == nullptr) {
        target_deck = 0;
        first_track = true;
    } else {
        target_deck = 1 - active_deck;
    }

    std::cout << "[Deck Switch] Target deck: " << target_deck << std::endl;

    if (decks[target_deck] != nullptr) {
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }

    clonetrack->load();
    clonetrack->analyze_beatgrid();

    if (auto_sync && decks[active_deck] != nullptr && !first_track) {
        if (!can_mix_tracks(clonetrack)) {
            sync_bpm(clonetrack);
        }
    }

    decks[target_deck] = clonetrack.release();
    std::cout << "[Load Complete] " << track.get_title() << " is now loaded on deck " << target_deck << std::endl;

    if (decks[active_deck] != nullptr && !first_track) {
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
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    // Verify that decks[active_deck] is not nullptr & wrapper contains a valid pointer
    if (decks[active_deck]==nullptr || !track) return false;
    // Get BPM values from both active deck and new track using get_bpm()
    int active_deck_bpm = decks[active_deck]->get_bpm();
    int new_track_bpm = track->get_bpm();
    // Calculate absolute BPM difference
    return (std::abs(active_deck_bpm - new_track_bpm) <= bpm_tolerance);
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    // Verify both active deck and new track are valid
    if (decks[active_deck]!=nullptr && track) {
        int active_deck_bpm = decks[active_deck]->get_bpm();
        int new_track_bpm = track->get_bpm();
        int bpm_avg = (active_deck_bpm + new_track_bpm) / 2;
        track->set_bpm(bpm_avg);
        std::cout << "[Sync BPM] Syncing BPM from  " << new_track_bpm << " to " << bpm_avg << "\n";
    }
}
