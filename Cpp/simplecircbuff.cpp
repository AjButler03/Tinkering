#include <iostream>
#include <vector>

using namespace std;

// Simple circular buffer of ints created using a vector.

// Note: this would not be memory safe if accessed in multithreading.
// That may be a fun thing to explore.

class CircBuff {
   protected:
    // vector with defined size for buffer
    vector<int>* buff;
    // Buffer Capacity
    int capacity;
    // idx to start reading from
    int readptr;
    // idx to start writing from
    int writeptr;
    // To flag if full of unread data
    bool full;

   public:
    // Constructor; requires capacity to immediately allocate buffer
    CircBuff(int cap) : capacity(cap), readptr(0), writeptr(0) {
        // Create Vector with cap capacity
        this->buff = new vector<int>(cap);
    };

    // Destructor; frees allocated buffer memory
    ~CircBuff() { delete this->buff; }

    // Write new int at write pointer
    // Returns idx where it was written
    int write(int value) {
        // write value to write idx
        this->buff->at(this->writeptr) = value;
        int idx = this->writeptr;
        // increment write idx
        this->writeptr = (this->writeptr + 1) % this->capacity;

        // Check: if write pointer has reached read pointer, then buff is full
        if (this->readptr == this->writeptr) {
            this->full = true;
        }

        // If already full, then we just overwrote oldest value to read.
        // Thus, increment read pointer.
        if (this->full) {
            this->readptr = (this->readptr + 1) % this->capacity;
        }

        return idx;
    };

    // Reads oldest unread data; increments read pointer afterward
    int read() {
        if (!this->full && this->readptr == this->writeptr) {
            // Buffer is empty.
            // For now, we'll just say 0.
            return 0;
        }

        int val = this->buff->at(this->readptr);

        this->readptr = (this->readptr + 1) % this->capacity;

        this->full = false;

        return val;
    }

    // Completely clears the buffer, resetting read/write pointers.
    void clear() {
        // Free existing vector
        delete this->buff;
        // Create new vector
        this->buff = new vector<int>(this->capacity);
        // Reset read/write
        this->writeptr = 0;
        this->readptr = 0;
        this->full = false;
    }
};

// Some simple tests for my simple buffer.
int main(int argc, char* argv[]) {
    CircBuff buff = CircBuff(5);

    for (int i = 0; i < 10; i++) {
        buff.write(i);
    };

    for (int i = 0; i < 10; i++) {
        cout << buff.read() << "\n";
    }
}