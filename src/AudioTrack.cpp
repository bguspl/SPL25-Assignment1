#include "AudioTrack.h"
#include <iostream>
#include <cstring>
#include <random>

AudioTrack::AudioTrack(const std::string& title, const std::vector<std::string>& artists, 
                      int duration, int bpm, size_t waveform_samples)
    : title(title), artists(artists), duration_seconds(duration), bpm(bpm), 
      waveform_size(waveform_samples) {

    // Allocate memory for waveform analysis
    waveform_data = new double[waveform_size];

    // Generate some dummy waveform data for testing
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    for (size_t i = 0; i < waveform_size; ++i) {
        waveform_data[i] = dis(gen);
    }
    #ifdef DEBUG
    std::cout << "AudioTrack created: " << title << " by " << std::endl;
    for (const auto& artist : artists) {
        std::cout << artist << " ";
    }
    std::cout << std::endl;
    #endif
}

// ========== TODO: STUDENTS IMPLEMENT RULE OF 5 ==========

// Destructor
AudioTrack::~AudioTrack() {
    #ifdef DEBUG
    std::cout << "AudioTrack destructor called for: " << title << std::endl;
    #endif

    //Checking if the waveform data points to null and if not, delete it
    if (waveform_data != nullptr){
        delete[] waveform_data;
    }
}

// Copy constructor
AudioTrack::AudioTrack(const AudioTrack& other)
    : title(other.title), 
      artists(other.artists),
      duration_seconds(other.duration_seconds), 
      bpm(other.bpm), 
      waveform_size(other.waveform_size)
{
    #ifdef DEBUG
    std::cout << "AudioTrack copy constructor called for: " << other.title << std::endl;
    #endif

    //Deep copy the waveform array to ower new element
    if(other.waveform_data != nullptr){
        waveform_data = new double[waveform_size];
        for (int i = 0;i<other.waveform_size;i++){
            waveform_data[i] = other.waveform_data[i];
        }
    }else{
        waveform_data = nullptr;
    }
}

// Copy assignment operator
AudioTrack& AudioTrack::operator=(const AudioTrack& other)
{
    #ifdef DEBUG
    std::cout << "AudioTrack copy assignment called for: " << other.title << std::endl;
    #endif

    //Check if other equals to the current element
    if (this == &other) {
        return *this;
    }

    //Updating the fields that are not pointers
    title = other.title;
    artists = other.artists;
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_size= other.waveform_size;

    //Destroying the array and building it from scratch    
    if (waveform_data != nullptr){
        delete[] waveform_data;
    }

    if (other.waveform_data != nullptr) {
        waveform_data = new double[waveform_size];
        for (size_t i = 0; i < waveform_size; i++) {
            waveform_data[i] = other.waveform_data[i];
        }
    }else{
        waveform_data = nullptr;
    }

    return *this;

}

// Move constructor
AudioTrack::AudioTrack(AudioTrack&& other) noexcept 
    : title(std::move(other.title)),
      artists(std::move(other.artists)),
      duration_seconds(other.duration_seconds),
      bpm(other.bpm),
      waveform_data(other.waveform_data),
      waveform_size(other.waveform_size) {
    #ifdef DEBUG
    std::cout << "AudioTrack move constructor called for: " << other.title << std::endl;
    #endif
    // Reseting all non-standart elemnts
    other.waveform_data = nullptr;
    other.waveform_size = 0;
}

// Move assignment operator
AudioTrack& AudioTrack::operator=(AudioTrack&& other) noexcept {
    #ifdef DEBUG
    std::cout << "AudioTrack move assignment called for: " << other.title << std::endl;
    #endif 
    // Updating the fields that are not pointers
    title = std::move(other.title);
    artists = std::move(other.artists);
    duration_seconds = other.duration_seconds;
    bpm = other.bpm;
    waveform_size= other.waveform_size;

    // Destroying the array   
    if (waveform_data != nullptr){
        delete[] waveform_data;
    }

    // Stealling the resources
    waveform_data = other.waveform_data;
    waveform_size = other.waveform_size;
    
    // Reseting all non-standart elemnts
    other.waveform_data = nullptr;   
    other.waveform_size = 0;

    return *this;
}

void AudioTrack::get_waveform_copy(double* buffer, size_t buffer_size) const {
    if (buffer && waveform_data && buffer_size <= waveform_size) {
        std::memcpy(buffer, waveform_data, buffer_size * sizeof(double));
    }
}