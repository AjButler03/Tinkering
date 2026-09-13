#include <iostream>
#include <stdexcept>

using namespace std;

// Building a simple int stack using an static array.
// Two goals:
// 1. Practice old school arrays
// 2. Basic manual memory allocation

// Static stack of ints, implemented using a basic array (not a vector, which
// would probably be better if this were serious)
class SimpleStack {
   protected:
    // pointer to array
    int* arr;
    // Maximum size
    int size;
    // idx of current top of stack
    int top;

   public:
    // Creates a new simple static stack of ints
    // Default size is 10
    SimpleStack(int size = 10) {
        // Allocate the array
        arr = new int[size];
        this->size = size;
        top = -1;
    };

    // Destructor
    ~SimpleStack() {
        // free allocated array
        delete[] arr;
    }

    // push a new value to the top of the stack
    // On success, returns current size of stack
    int push(int value) {
        // Check that there is room first
        if (this->top > this->size) {
            // Stack overflow; throw exception
            throw length_error("Stack Overflow; stack has no available room.");
        }
        // otherwise, add the element to the stack
        this->arr[this->top + 1] = value;
        this->top++;
        return this->top + 1;
    }

    // pop a value off the top of the stack
    int pop() {
        // Check that there is a value to pop
        if (this->top >= 0) {
            int value = this->arr[this->top];
            this->top--;
            return value;
        } else {
            // Probably a more ideal exception to throw, but this is fine
            throw length_error("No element to pop; stack is empty.");
        }
    }

    // Clears stack
    // Note that this doesn't actually clear values, just resets 'top' idx
    void clear() { this->top = -1; }

    // Check if stack is full
    bool full() { return (this->top + 1 >= this->size); }

    // check if stack is empty
    bool empty() { return (this->top < 0); }

    // returns current stack height
    int height() { return this->top + 1; }
};

int main(int argc, char* argv[]) {
    SimpleStack ss = SimpleStack();

    for (int i = 0; i < 6; i++) {
        ss.push(i);
    }

    cout << "Stack size: " << ss.height() << "\n";
    cout << "Stack empty: " << ss.empty() << "\n";
    cout << "Stack full: " << ss.full() << "\n";

    while (!ss.empty()) {
        cout << ss.pop() << "\n";
    };

    cout << "Stack empty: " << ss.empty() << "\n";
    cout << "Stack size: " << ss.height() << "\n";
}