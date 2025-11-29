#ifndef POINTERWRAPPER_H
#define POINTERWRAPPER_H

#include <utility>
#include <iostream>

/**
 * PointerWrapper - A template class that wraps a raw pointer
 * 
 * This is Phase 3 of the assignment. Students must analyze and implement
 * a complete pointer wrapper class that manages dynamic memory safely.
 * 
 * Refer to the assignment instructions (Phase 3) for detailed guiding questions
 * about resource management, ownership semantics, copy vs move, and interface design.
 */
template<typename T>
class PointerWrapper {
private:
    T* ptr;  // Raw pointer to the managed object
    
public:
    // ========== CONSTRUCTION AND DESTRUCTION ==========

    /**
     * Default constructor - creates empty wrapper
     */
    PointerWrapper() : ptr(nullptr) {}

    /**
     * Constructor from raw pointer - wraps the pointer
     */
    explicit PointerWrapper(T* p) : ptr(p) {}

    /**
     * HINT: What should happen to the wrapped pointer when the wrapper is destroyed?
     * Think about ownership and resource management.
     * Is the default destructor sufficient here?
     */
    ~PointerWrapper() {
        if (ptr != nullptr) {
            delete ptr;
        }
    }

    // ========== COPY OPERATIONS (DELETED) ==========

    /**
     * Copy constructor is DELETED
     * Think about why this might be necessary for a pointer wrapper
     */
    PointerWrapper(const PointerWrapper& other) = delete;

    /**
     * Copy assignment is DELETED
     * Consider what problems could arise if copying was allowed
     */
    PointerWrapper& operator=(const PointerWrapper& other) = delete;

    // ========== MOVE OPERATIONS (STUDENTS IMPLEMENT) ==========

    /**
     * HINT: How should ownership transfer from one wrapper to another?
     * What should happen to the source wrapper after the move?
     */
    PointerWrapper(PointerWrapper&& other) noexcept:ptr(other.ptr) {
        other.ptr = nullptr;
    }
    /**
     * HINT: Handle cleanup of current resource and ownership transfer
     * Don't forget about self-assignment!
     */
    PointerWrapper& operator=(PointerWrapper&& other) noexcept {
        if(other.ptr==ptr) {
            return *this;
        }
        if(ptr != nullptr) {
            delete ptr;
        }
        ptr = other.ptr;
        other.ptr = nullptr;
        return *this;
    }

    // ========== ACCESS OPERATIONS ==========

    /**
     * HINT: How do you access the object that the wrapper points to? Is this operation implementation complete?
     * @throws std::runtime_error if ptr is null
     */

    T& operator*() const {
        if(ptr == nullptr) {
            throw std::runtime_error("Pointer is null");
        }
        return *ptr;
    };

    /**
     * HINT: How do you access members of the wrapped object?
     * What safety checks should you perform?
     */
    T* operator->() const {
        if(ptr == nullptr) {
            throw std::runtime_error("Pointer is null");
        }
        return ptr;
    }
    // main PointerWrapper<Mp3> object(new Mp3Track("title", "artist", 100, 120, 128, true));
    // object->get_title()

    /**
     * HINT: Sometimes you need access to the raw pointer without changing ownership
     * What should this function return?
     * @throws std::runtime_error if ptr is null
     */
    T* get() const {
        if(ptr == nullptr) {
            throw std::runtime_error("Pointer is null");
        }
        return ptr;
    }

    // ========== OWNERSHIP MANAGEMENT ==========

    /**
     * HINT: What does "release" mean in terms of ownership?
     * Should the wrapper still own the pointer after calling release()?
     */
    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    /**
     * HINT: How do you replace the currently wrapped pointer?
     * What should happen to the old pointer?
     */

    void reset(T* new_ptr = nullptr) {
        if(new_ptr == ptr) {
            return;
        }
        if(ptr != nullptr) {
            delete ptr;
        }
        ptr = new_ptr;
    }

    // ========== UTILITY FUNCTIONS ==========

    /**
     * HINT: When should a wrapper be considered "true" or "false"?
     * Why might the explicit keyword be important here?
     */


    explicit operator bool() const {
        return ptr != nullptr;
    }

    /**
     * Swap two PointerWrapper objects
     * This is implemented for you as a reference
     */
    void swap(PointerWrapper& other) noexcept {
        std::swap(ptr, other.ptr);
    }
};

// ========== NON-MEMBER FUNCTIONS ==========

/**
 * Helper function to create PointerWrapper
 * This is implemented for you as an example
 * Can you figure out when this would be useful in phase 4?
 */
template<typename T, typename... Args>
PointerWrapper<T> make_pointer_wrapper(Args&&... args) {
    return PointerWrapper<T>(new T(std::forward<Args>(args)...));
}

/**
 * HINT: How can you swap two wrapper objects?
 * Why might this be useful?
 */
template<typename T>
void swap(PointerWrapper<T>& lhs, PointerWrapper<T>& rhs) noexcept {
    // HINT: You can use the member swap function
    //your code here...
    if(&lhs == &rhs) {
        return;
    }
    lhs.swap(rhs);
}

#endif // POINTERWRAPPER_H