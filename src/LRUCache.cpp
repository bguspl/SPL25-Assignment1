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
    if(track.get() == nullptr){
        return false;
    }
    for (CacheSlot& element : slots){
        if(element.isOccupied()){
            AudioTrack* el = element.getTrack();
            AudioTrack* tr = track.get();
            if(el->get_title() == tr->get_title()){
                el = element.access(access_counter);
                access_counter++;
                return false;
            }
        }
    }
    if(slots.size() == max_size){
        bool ans = evictLRU();
        size_t index = findEmptySlot();
        slots.at(index).store(std::move(track), access_counter);
        return true;
    }
    size_t index = findEmptySlot();
    slots.at(index).store(std::move(track), access_counter);
    return false; // Placeholder
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
    if(size() == 0){
        return max_size;
    }
    int index = 0;
    int counter = -1;
    uint64_t min  = -1;
    for (const CacheSlot& element : slots){
        counter++;
        if(element.isOccupied()){
            if(min < 0){
                min = element.getLastAccessTime();
                index = counter;
            }
            else if(element.getLastAccessTime() < min){
                min = element.getLastAccessTime();
                index = counter;
            }
        }
    }
    return index; // Placeholder
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