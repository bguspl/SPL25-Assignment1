#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void WAVTrack::load() {

    // Implementation of Wav loading with format-specific operations
    std::cout << "[WAVTrack::load] Loading WAV: \"" << title
            << "\" at " << sample_rate << "Hz/ " << bit_depth << " bit (uncompressed)..." << std::endl;
    long long size = duration_seconds * sample_rate * (bit_depth / 8) * 2;
    std::cout << "  → Estimated file size: " << size << " bytes" << std::endl;
    std::cout << "  → Fast loading due to uncompressed format." << std::endl;
}

void WAVTrack::analyze_beatgrid() {

    // Implementation of analyze beatgrid with format-specific operations
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    int beats = (duration_seconds / 60.0) * bpm;
    std::cout << "  → Estimated beats: " << beats <<"  → Precision factor: 1 (uncompressed audio) " << std::endl;

}

double WAVTrack::get_quality_score() const {
    
    // Implementation of get quality with format-specific operations
    int score = 70;

    if(sample_rate >= 44100){
        score = score + 10;
    }

    if(sample_rate >= 96000){
        score = score +5;
    }

    if (bit_depth >= 16){
        score = score + 10;
    }

    if(bit_depth >= 24){
        score = score +5;
    }

    if(score > 100){
        score = 100;
    }

    return score;
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {
    // TODO: Implement the clone method
    return PointerWrapper<AudioTrack>(nullptr); // Replace with your implementation
}