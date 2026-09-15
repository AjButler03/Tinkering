#include <iostream>
#include <vector>

using namespace std;

// Simple circular buffer of ints created using a vector.

class CircBuff {
   protected:
    // vector with defined size for buffer
    vector<int>* buff;
    // Buffer Capacity
    int capacity;
    // idx to start reading from
    int read;
    // idx to start writing from
    //
    int write;
    // To flag if full
    bool full;

   public:
    // Constructor; requires capacity to immediately allocate buffer
    CircBuff(int cap) : capacity(cap), read(0), write(0) {
        // Create Vector with cap capacity
        this->buff = new vector<int>(cap);
    };

    // Destructor; frees allocated buffer memory
    ~CircBuff() { delete this->buff; }

    // Write new int at write pointer
    // Returns idx where it was written
    int write(int value) {
        // write value to write idx
        this->buff->at(this->write) = value;
        int idx = this->write;
        // increment write idx
        this->write = (this->write + 1) % this->capacity;

        // Check: if idx overwrote oldest data (read), then increment read
        if (this->read == idx) {
            this->read = (this->read + 1) % this->capacity;
        }

        return idx;
    };
};

int main(int argc, char* argv[]) {}