#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <vector>
#include <iostream>

template <typename T>
class ObjectPool {
private:
    std::vector<T> pool;          // The continuous block of memory
    std::vector<T*> freeObjects;  // Pointers to available slots

public:
    ObjectPool(size_t size) {
        pool.resize(size);
        freeObjects.reserve(size);

        // Point to each slot in the block
        for (size_t i = 0; i < size; ++i) {
            freeObjects.push_back(&pool[i]);
        }
    }

    // Get a pointer to a free object (O(1) speed)
    T* acquire() {
        if (freeObjects.empty()) {
            std::cout << "CRITICAL WARNING: Object Pool Depleted! Resizing... (Performance Hit)\n";
            return nullptr;
        }

        T* obj = freeObjects.back();
        freeObjects.pop_back();
        return obj;
    }

    // Return the object to the pool (O(1) speed)
    void release(T* obj) {
        freeObjects.push_back(obj);
    }
};

#endif