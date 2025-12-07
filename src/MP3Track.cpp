#include "MP3Track.h"
#include <iostream>
#include <cmath>
#include <algorithm>

MP3Track::MP3Track(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int bitrate, bool has_tags)
    : AudioTrack(title, artists, duration, bpm), bitrate(bitrate), has_id3_tags(has_tags) {

    std::cout << "MP3Track created: " << bitrate << " kbps" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void MP3Track::load() {
    std::cout << "[MP3Track::load] Loading MP3: \"" << title
              << "\" at " << bitrate << " kbps...\n";
    // TODO: Implement MP3 loading with format-specific operations
    // NOTE: Use exactly 2 spaces before the arrow (→) character
    if(has_id3_tags == true) {
        std::cout << "--> Processing ID3 metadata (artist info, album art, etc.)...";
     }
     else {
         std::cout << "No ID3 tags found";
     }

    std::cout << "--> Decoding MP3 frames";
    std::cout << "--> Load complete";


    
}

void MP3Track::analyze_beatgrid() {
     std::cout << "[MP3Track::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    // TODO: Implement MP3-specific beat detection analysis
    // NOTE: Use exactly 2 spaces before each arrow (→) character
    double beats = ((double)duration_seconds / 60.0) * bpm;
    double precision_factor = (double)bitrate / 320.0;

    std::cout << " -> Estimated beats: " << beats 
              << " -> Compression precision factor: " << precision_factor 
              << std::endl;


}

double MP3Track::get_quality_score() const {
    // TODO: Implement comprehensive quality scoring
    // NOTE: This method does NOT print anything

    double final_score = ((double)bitrate / 320.0) * 100.0;
    
    if (has_id3_tags == true) {
        final_score += 5.0;
    }
    
    if (bitrate < 128) {
        final_score -= 10.0;
    }
    
    final_score = std::max(0.0, final_score);
    final_score = std::min(100.0, final_score);
    return final_score;
}

PointerWrapper<AudioTrack> MP3Track::clone() const {
    // TODO: Implement polymorphic cloning
    MP3Track* cloned_track = new MP3Track(*this);
    return PointerWrapper<AudioTrack>(cloned_track); // Replace with your implementation
}