#include "MixingEngineService.h"
#include <iostream>
#include <memory>

MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0] = nullptr;
    decks[1] = nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
}

// Implementing rule of 5

MixingEngineService::~MixingEngineService()
{
    std::cout << "[MixingEngineService] Cleaning up decks..." << std::endl;

    for (int i = 0; i < 2; i++)
    {
        if (decks[i])
        {
            delete decks[i];
            decks[i] = nullptr;
        }
    }
}

MixingEngineService::MixingEngineService(const MixingEngineService &other)
    : active_deck(other.active_deck), auto_sync(other.auto_sync),
      bpm_tolerance(other.bpm_tolerance)
{
    for (int i = 0; i < 2; i++)
    {
        if (other.decks[i] == nullptr)
        {
            decks[i] = nullptr;
        }
        else
        {
            decks[i] = other.decks[i]->clone().release();
        }
    }
}

MixingEngineService &MixingEngineService::operator=(const MixingEngineService &other)
{
    // Check if other equals to the current element
    if (this == &other)
    {
        return *this;
    }

    // Updating the fields that are not pointers
    active_deck = other.active_deck;
    auto_sync = other.auto_sync;
    bpm_tolerance = other.bpm_tolerance;

    // Destroying the current track in decks
    for (int i = 0; i < 2; i++)
    {
        if (decks[i])
        {
            delete decks[i];
            decks[i] = nullptr;
        }
    }

    // Updating decks and releasing the raw pointer from PointerWrapper
    for (int i = 0; i < 2; i++)
    {
        if (other.decks[i] != nullptr)
        {
            decks[i] = other.decks[i]->clone().release();
        }
        else
        {
            decks[i] = nullptr;
        }
    }
    return *this;
}

// Move Constructor
MixingEngineService::MixingEngineService(MixingEngineService &&other) noexcept
    : active_deck(other.active_deck), auto_sync(other.auto_sync),
      bpm_tolerance(other.bpm_tolerance)
{
    // Stealing recources
    decks[0] = other.decks[0];
    decks[1] = other.decks[1];

    // Making sure we delete other
    other.decks[0] = nullptr;
    other.decks[1] = nullptr;
}

// Move Assignment Operator
MixingEngineService &MixingEngineService::operator=(MixingEngineService &&other) noexcept
{
    // Checking for equality
    if (this == &other)
    {
        return *this;
    }

    // Clean up current resources
    for (int i = 0; i < 2; i++)
    {
        if (decks[i])
        {
            delete decks[i];
            decks[i] = nullptr;
        }
    }

    // Steal resources from other
    decks[0] = other.decks[0];
    decks[1] = other.decks[1];
    active_deck = other.active_deck;
    auto_sync = other.auto_sync;
    bpm_tolerance = other.bpm_tolerance;

    // Making sure we delete other
    other.decks[0] = nullptr;
    other.decks[1] = nullptr;

    return *this;
}

/**
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack &track)
{
    std::cout << "\n=== Loading Track to Deck ===" << std::endl;

    // Wrapping the track in a PointerWrapper
    PointerWrapper<AudioTrack> track_clone = track.clone();

    // Check if the clone worked
    if (!track_clone)
    {
        std::cerr << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone" << std::endl;
        return -1;
    }

    // Identifing tartget deck
    int target_deck;
    if (decks[0] == nullptr && decks[1] == nullptr)
    {
        target_deck = 0;
    }
    else if (active_deck == 1)
    {
        target_deck = 0;
    }
    else
    {
        target_deck = 1;
    }

    // Log output
    std::cout << "[Deck Switch] Target deck: " << target_deck << std::endl;

    // Deleting the track in target deck
    if (decks[target_deck])
    {
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }

    // Performing a track preparation on cloned track
    track_clone->load();
    track_clone->analyze_beatgrid();

    // BPM Management
    if (auto_sync)
    {
        if (decks[active_deck]) // other deck has a track
        {
            if (!can_mix_tracks(track_clone)) 
            {
                sync_bpm(track_clone);
            }
        }
        else  // Other deck is empty (first track case) - this was not asked but matches the output.
        {
            std::cout << "[Sync BPM] Cannot sync - one of the decks is empty." << std::endl;
        }
    }

    // Release pointer from pointer wrapper and assign to target deck
    AudioTrack *raw_ptr = track_clone.release();
    decks[target_deck] = raw_ptr;
    std::cout << "[Load Complete] '" << raw_ptr->get_title() << "' is now loaded on deck " << target_deck << std::endl;

    active_deck = target_deck;

    std::cout << "[Active Deck] Switched to deck " << target_deck << std::endl;

    return target_deck;
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const
{
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i)
    {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack> &track) const
{
    // Verify that deck is not null
    if (!decks[active_deck])
    {
        return false;
    }

    // Verify that the input track wrapper contains a valid pointer
    if (!track.get())
    {
        return false;
    }

    // Calculate abs BPM
    int active_deck_bpm = decks[active_deck]->get_bpm();
    int new_track_bpm = track->get_bpm();
    if (std::abs(active_deck_bpm - new_track_bpm) <= bpm_tolerance)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack> &track) const
{
    // Verify both active deck and new track are valid
    if (decks[active_deck] && track)
    {

        // Get original BPM
        int new_track_bpm = track->get_bpm();

        // Get original BPM
        int average_bpm = (new_track_bpm + decks[active_deck]->get_bpm()) / 2;
        track->set_bpm(average_bpm);

        std::cout << "[Sync BPM] Syncing BPM from " << new_track_bpm << " to " << track->get_bpm() << std::endl;
    }
}
