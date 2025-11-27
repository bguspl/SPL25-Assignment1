#include "Playlist.h"
#include "AudioTrack.h"
#include <iostream>
#include <algorithm>
Playlist::Playlist(const std::string& name) 
    : head(nullptr), playlist_name(name), track_count(0) {
    std::cout << "Created playlist: " << name << std::endl;
}

// =========================================================
// Rule of 3 Implementation for Playlist
// =========================================================

// TODO: Fix memory leaks!
// Students must fix this in Phase 1
Playlist::~Playlist() {
    #ifdef DEBUG
    std::cout << "Destroying playlist: " << playlist_name << std::endl;
    #endif

    PlaylistNode* current = head; //start from the head
    PlaylistNode* next_node = nullptr;

    while (current != nullptr) { //iterating through all the list
        next_node = current->next; //always keeping the next_node pointer
        delete current-> track; //in phase 4 we found out that each playlist holds cloned_tracks, therefore their deletion will happen during playlist destruction
        delete current; //delete the current node we're on
        current = next_node; //moving to the next node
    }

    head = nullptr; //restart the head
}

// Copy Constructor
Playlist::Playlist(const Playlist& other) 
    : head(nullptr), playlist_name(other.playlist_name), track_count(0) {
    
    // Start iterating from the source head
    PlaylistNode* current_source = other.head;
    
    // Keep track of the last added node for efficient linking
    PlaylistNode* last_added = nullptr;

    while (current_source != nullptr) {
        // Create a new Node (Deep Copy of the structure)
        // But pass the SAME track pointer (Shallow Copy of the content / Borrowing)
        PlaylistNode* new_node = new PlaylistNode(current_source->track);

        if (head == nullptr) {
            head = new_node; // Set the first node
        } else {
            last_added->next = new_node; // Link to the previous node
        }

        // Advance pointers
        last_added = new_node;
        current_source = current_source->next;
        track_count++;
    }
}

// Copy Assignment Operator
Playlist& Playlist::operator=(const Playlist& other) {
    // 1. Check for self-assignment
    if (this == &other) {
        return *this; 
    }

    // 2. Clean up existing resources (same logic as destructor)
    PlaylistNode* current = head;
    while (current != nullptr) {
        PlaylistNode* next_node = current->next;
        delete current; // Delete the Node, NOT the Track
        current = next_node;
    }
    head = nullptr;
    track_count = 0;

    // 3. Copy data from other (same logic as Copy Constructor)
    playlist_name = other.playlist_name;
    
    PlaylistNode* current_source = other.head;
    PlaylistNode* last_added = nullptr;

    while (current_source != nullptr) {
        // Create new node referencing the same track
        PlaylistNode* new_node = new PlaylistNode(current_source->track);

        if (head == nullptr) {
            head = new_node;
        } else {
            last_added->next = new_node;
        }

        last_added = new_node;
        current_source = current_source->next;
        track_count++;
    }

    return *this;
}

void Playlist::add_track(AudioTrack* track) {
    if (!track) {
        std::cout << "[Error] Cannot add null track to playlist" << std::endl;
        return;
    }

    // Create new node - this allocates memory!
    PlaylistNode* new_node = new PlaylistNode(track);

    // Add to front of list
    new_node->next = head;
    head = new_node;
    track_count++;

    std::cout << "Added '" << track->get_title() << "' to playlist '" 
              << playlist_name << "'" << std::endl;
}

void Playlist::remove_track(const std::string& title) {
    PlaylistNode* current = head;
    PlaylistNode* prev = nullptr;

    // Find the track to remove
    while (current && current->track->get_title() != title) {
        prev = current;
        current = current->next;
    }

    if (current) {
        // Remove from linked list
        if (prev) {
            prev->next = current->next;
        } else {
            head = current->next;
        }
        
        //delete the node
        delete current->track; //in phase 4 we found out that each playlist holds cloned_tracks, therefore their deletion will happen during playlist destruction
        delete current;

        track_count--;
        std::cout << "Removed '" << title << "' from playlist" << std::endl;

    } else {
        std::cout << "Track '" << title << "' not found in playlist" << std::endl;
    }
}

void Playlist::display() const {
    std::cout << "\n=== Playlist: " << playlist_name << " ===" << std::endl;
    std::cout << "Track count: " << track_count << std::endl;

    PlaylistNode* current = head;
    int index = 1;

    while (current) {
        std::vector<std::string> artists = current->track->get_artists();
        std::string artist_list;

        std::for_each(artists.begin(), artists.end(), [&](const std::string& artist) {
            if (!artist_list.empty()) {
                artist_list += ", ";
            }
            artist_list += artist;
        });

        AudioTrack* track = current->track;
        std::cout << index << ". " << track->get_title() 
                  << " by " << artist_list
                  << " (" << track->get_duration() << "s, " 
                  << track->get_bpm() << " BPM)" << std::endl;
        current = current->next;
        index++;
    }

    if (track_count == 0) {
        std::cout << "(Empty playlist)" << std::endl;
    }
    std::cout << "========================\n" << std::endl;
}

AudioTrack* Playlist::find_track(const std::string& title) const {
    PlaylistNode* current = head;

    while (current) {
        if (current->track->get_title() == title) {
            return current->track;
        }
        current = current->next;
    }

    return nullptr;
}

int Playlist::get_total_duration() const {
    int total = 0;
    PlaylistNode* current = head;

    while (current) {
        total += current->track->get_duration();
        current = current->next;
    }

    return total;
}

std::vector<AudioTrack*> Playlist::getTracks() const {
    std::vector<AudioTrack*> tracks;
    PlaylistNode* current = head;
    while (current) {
        if (current->track)
            tracks.push_back(current->track);
        current = current->next;
    }
    return tracks;
}