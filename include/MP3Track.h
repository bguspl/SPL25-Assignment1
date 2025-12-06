#ifndef MP3TRACK_H
#define MP3TRACK_H

#include "AudioTrack.h"

/**
 * MP3Track - Represents an MP3 audio file with lossy compression
 * MP3 uses perceptual coding to reduce file size while maintaining acceptable quality
 * Students must implement all virtual functions from AudioTrack
 *
 * Phase 4 contracts:
 * - load(): simulate deck preparation (format-specific message); does not start playback.
 * - analyze_beatgrid(): run immediately after load() in this assignment for compatibility checks.
 * - get_quality_score(): derived from bitrate (e.g., normalized by 320kbps).
 * - clone(): return a deep polymorphic copy used by the mixer; source remains unchanged.
 */
class MP3Track : public AudioTrack
{
private:
    int bitrate;       // Compression level: 128, 192, 320 kbps (higher = better quality)
    bool has_id3_tags; // Whether file contains ID3 metadata (artist, album, etc.)

public:
    /**
     * Constructor for MP3Track
     */
    MP3Track(const std::string &title, const std::vector<std::string> &artists,
             int duration, int bpm, int bitrate, bool has_tags = true);

    void load() override;

    void analyze_beatgrid() override;

    double get_quality_score() const override;

    PointerWrapper<AudioTrack> clone() const override;

    // Getters
    int get_bitrate() const { return bitrate; }
    bool has_tags() const { return has_id3_tags; }
};

#endif // MP3TRACK_H