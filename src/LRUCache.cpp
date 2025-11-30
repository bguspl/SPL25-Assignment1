#include "LRUCache.h"
#include <iostream>

LRUCache::LRUCache(size_t capacity)
    : slots(capacity), max_size(capacity), access_counter(0) {}

bool LRUCache::contains(const std::string &track_id) const
{
    return findSlot(track_id) != max_size;
}

AudioTrack *LRUCache::get(const std::string &track_id)
{
    size_t idx = findSlot(track_id);
    if (idx == max_size)
        return nullptr;
    return slots[idx].access(++access_counter);
}

bool LRUCache::put(PointerWrapper<AudioTrack> track)
{
    bool ans = false;

    // Safety check: ignore null pointers
    if (!track)
    {
        return false;
    }

    std::string track_title = track->get_title();

    // If it's already here, just mark it as 'recently used' (MRU) and we're done
    if (contains(track_title))
    {
        get(track_title);
        return false;
    }

    // Evict the oldest item to make room
    if (isFull())
    {
        evictLRU();
        ans = true; // Make ans true if eviction happend
    }

    // Find an empty slot
    size_t empty_slot = findEmptySlot();

    // Move ownership of the track here and stamp it with the current access_counter
    slots[empty_slot].store(std::move(track), ++access_counter);

    return ans;
}

bool LRUCache::evictLRU()
{
    size_t lru = findLRUSlot();
    if (lru == max_size || !slots[lru].isOccupied())
        return false;
    slots[lru].clear();
    return true;
}

size_t LRUCache::size() const
{
    size_t count = 0;
    for (const auto &slot : slots)
        if (slot.isOccupied())
            ++count;
    return count;
}

void LRUCache::clear()
{
    for (auto &slot : slots)
    {
        slot.clear();
    }
}

void LRUCache::displayStatus() const
{
    std::cout << "[LRUCache] Status: " << size() << "/" << max_size << " slots used\n";
    for (size_t i = 0; i < max_size; ++i)
    {
        if (slots[i].isOccupied())
        {
            std::cout << "  Slot " << i << ": " << slots[i].getTrack()->get_title()
                      << " (last access: " << slots[i].getLastAccessTime() << ")\n";
        }
        else
        {
            std::cout << "  Slot " << i << ": [EMPTY]\n";
        }
    }
}

size_t LRUCache::findSlot(const std::string &track_id) const
{
    for (size_t i = 0; i < max_size; ++i)
    {
        if (slots[i].isOccupied() && slots[i].getTrack()->get_title() == track_id)
            return i;
    }
    return max_size;
}

size_t LRUCache::findLRUSlot() const
{
    // Initialize min_access_time as max number
    uint64_t min_access_time = UINT64_MAX;
    size_t lru_index = max_size;

    // If slots isnt empty, iterate over slots to find min_access_time
    if (!slots.empty())
    {
        for (size_t i = 0; i < max_size; i++)
        {
            // Check that the slot is not empty
            if (slots[i].isOccupied())
            {
                uint64_t last_access_time = slots[i].getLastAccessTime();
                if (last_access_time < min_access_time)
                {
                    min_access_time = last_access_time;
                    lru_index = i;
                }
            }
        }
        return lru_index;
    }
    else
    {
        return lru_index;
    }
}

size_t LRUCache::findEmptySlot() const
{
    for (size_t i = 0; i < max_size; ++i)
    {
        if (!slots[i].isOccupied())
            return i;
    }
    return max_size;
}

void LRUCache::set_capacity(size_t capacity)
{
    if (max_size == capacity)
        return;
    // udpate max size
    max_size = capacity;
    // update the slots vector
    slots.resize(capacity);
}