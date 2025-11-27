#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void WAVTrack::load() {
    std::cout << "[WAVTrack::load] Loading WAV: \"" << title << "\" at " << sample_rate << "Hz/" << bit_depth << "bit (uncompressed)...\n";
    long size = duration_seconds * sample_rate * (bit_depth / 8) * 2 ;
    std::cout << "  -> Estimated file size: " << size << " bytes" << std::endl;
    std::cout << "  -> Fast loading due to uncompressed format." << std::endl;
}

void WAVTrack::analyze_beatgrid() {
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    double beats_estimated = (duration_seconds / 60.0) * bpm;
    double precision_factor = 1.0;
    std::cout << "  -> Estimated beats: " << beats_estimated << "  -> Precision factor: " << precision_factor << " (uncompressed audio)" << std::endl;
}

double WAVTrack::get_quality_score() const {
    double base_score = 70.0;
    if(sample_rate >= 44100) {
        base_score += 10;
        if(sample_rate >= 96000) {
            base_score += 5;
        }
    }
    if(bit_depth >= 16) {
        base_score += 10;
        if(bit_depth >= 24) {
            base_score += 5;
        }
    }
    if(base_score > 100) {
        base_score = 100;
    }
    return base_score;
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {
    // TODO: Implement the clone method
    return PointerWrapper<AudioTrack>(nullptr); // Replace with your implementation
}