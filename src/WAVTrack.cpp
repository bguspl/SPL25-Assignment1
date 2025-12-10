#include "WAVTrack.h"
#include <iostream>
#include <cmath>
#include <algorithm>

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void WAVTrack::load() {
     // TODO: Implement realistic WAV loading simulation
    // NOTE: Use exactly 2 spaces before the arrow (→) character

    std::cout << "[WAVTrack::load] Loading WAV: \"" << title 
              << "\" at " << sample_rate << "Hz/" << bit_depth << "bit (uncompressed)...\n";


    long long size = (long long)duration_seconds * sample_rate * (bit_depth / 8) * 2;
    std::cout << "  → Estimated file size: " << size << " bytes\n";

    std::cout << "  → Fast loading due to uncompressed format.\n";
}

void WAVTrack::analyze_beatgrid() {
     // TODO: Implement WAV-specific beat detection analysis
    // Requirements:
    // 1. Print analysis message with track title
    // 2. Calculate beats: (duration_seconds / 60.0) * bpm
    // 3. Print number of beats and mention uncompressed precision
    // should print "  → Estimated beats: <beats>  → Precision factor: 1.0 (uncompressed audio)"
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    
    double beats = (duration_seconds / 60.0) * bpm;

    std::cout << "  → Estimated beats: " << (int)beats 
              << "  → Precision factor: 1 (uncompressed audio)" << std::endl;
}

double WAVTrack::get_quality_score() const {
      // TODO: Implement WAV quality scoring
    // NOTE: Use exactly 2 spaces before each arrow (→) character
    // NOTE: Cast beats to integer when printing
    double score = 70.0;
    if (sample_rate >= 44100) score += 10;
    if (sample_rate >= 96000) score += 5;
    if (bit_depth >= 16) score += 10;
    if (bit_depth >= 24) score += 5;
    if (score > 100.0) score = 100.0;
    

    #ifdef DEBUG
    std::cout << "Quality: [WAVTrack::get_quality_score] \"" << title << "\" score = " << (int)score << "/100" << std::endl;
    #endif

    return score;
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {
      // TODO: Implement the clone method
    return PointerWrapper<AudioTrack>(new WAVTrack(*this));// Replace with your implementation
}