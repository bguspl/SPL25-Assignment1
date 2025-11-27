#include "LRUCache.h"
#include <iostream>

LRUCache::LRUCache(size_t capacity)
    : slots(capacity), max_size(capacity), access_counter(0) {}

bool LRUCache::contains(const std::string& track_id) const {
    return findSlot(track_id) != max_size;
}

AudioTrack* LRUCache::get(const std::string& track_id) {
    size_t idx = findSlot(track_id);
    if (idx == max_size) return nullptr;
    return slots[idx].access(++access_counter);
}

/**
 * TODO: Implement the put() method for LRUCache
 */
bool LRUCache::put(PointerWrapper<AudioTrack> track) {
    bool evicted = false;

    // Handle nullptr track by returning immediately
    if (!track) {
        return evicted;
    }

    // Get the title of the new track once
    std::string new_track_title = track->get_title();

    // Check if a track with the same title already exists in the cache
    for (size_t i = 0; i < max_size; ++i) {
        if (slots[i].isOccupied()) {
            AudioTrack* existing_track = slots[i].getTrack();
            if (existing_track->get_title() == new_track_title) {
                slots[i].access(++access_counter);
                return evicted;
            }
        }
    }
    // If the cache is full (all slots occupied), call evictLRU() first
    // Find an empty slot using findEmptySlot()
    // Store the new track with the current access_counter value and mark the slot as occupied
    // Return true if an eviction occurred
    if (isFull()) {
        evicted = evictLRU();
    }
    size_t idx = findEmptySlot();
    slots[idx].store(std::move(track), ++access_counter);
    return evicted;
}

bool LRUCache::evictLRU() {
    size_t lru = findLRUSlot();
    if (lru == max_size || !slots[lru].isOccupied()) return false;
    slots[lru].clear();
    return true;
}

size_t LRUCache::size() const {
    size_t count = 0;
    for (const auto& slot : slots) if (slot.isOccupied()) ++count;
    return count;
}

void LRUCache::clear() {
    for (auto& slot : slots) {
        slot.clear();
    }
}

void LRUCache::displayStatus() const {
    std::cout << "[LRUCache] Status: " << size() << "/" << max_size << " slots used\n";
    for (size_t i = 0; i < max_size; ++i) {
        if(slots[i].isOccupied()){
            std::cout << "  Slot " << i << ": " << slots[i].getTrack()->get_title()
                      << " (last access: " << slots[i].getLastAccessTime() << ")\n";
        } else {
            std::cout << "  Slot " << i << ": [EMPTY]\n";
        }
    }
}

size_t LRUCache::findSlot(const std::string& track_id) const {
    for (size_t i = 0; i < max_size; ++i) {
        if (slots[i].isOccupied() && slots[i].getTrack()->get_title() == track_id) return i;
    }
    return max_size;

}

/**
 * TODO: Implement the findLRUSlot() method for LRUCache
 */
size_t LRUCache::findLRUSlot() const {
    
    size_t lru_slot = max_size;
    unsigned long min_access_time = 0;
    bool found_first_occupied = false;

    for (size_t i = 0; i < max_size; ++i) {
        
        if (slots[i].isOccupied()) {
            
            if (!found_first_occupied) {
                min_access_time = slots[i].getLastAccessTime();
                lru_slot = i;
                found_first_occupied = true;
            } 
            else {
                if (slots[i].getLastAccessTime() < min_access_time) {
                    // Found a new minimum
                    min_access_time = slots[i].getLastAccessTime();
                    lru_slot = i;
                }
            }
        }
    }

    return lru_slot;
}

size_t LRUCache::findEmptySlot() const {
    for (size_t i = 0; i < max_size; ++i) {
        if (!slots[i].isOccupied()) return i;
    }
    return max_size;
}

void LRUCache::set_capacity(size_t capacity){
    if (max_size == capacity)
        return;
    //udpate max size
    max_size = capacity;
    //update the slots vector
    slots.resize(capacity);
}