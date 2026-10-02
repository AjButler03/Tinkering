#include <iostream>

// Part 1 fix
// Original getvalue() declared an int object in
// the scope of the function, which no longer exists on return.
// Thus, it returns a dangling pointer - i.e., the pointer points to
// an unallocated area of memory.
// int* getValue()
// {
//     int* value = new int(42);
//     return value;
// }

// int main()
// {
//     int* ptr = getValue();

//     std::cout << *ptr << "\n";

//     return 0;
// }

// Part2
// Attempting to return by reference doesn't work either
// This is essentially saying that you want to return the exact
// int instance created within getValue(), which is again
// deleted when it returns. Thus, the int object also doesn't
// exist anymore, causing problems.
// Fix: return a copy of the int object (i.e., just remove the &)
// int getValue()
// {
//     int value = 42;
//     return value;
// }

// int main()
// {
//     int ref = getValue();

//     std::cout << ref << "\n";

//     return 0;
// }

// problem here is that
// buffer b = a
// simply copies the stored values in a to b
// a stores a pointer to an area in memory; b gets that same pointer
// so both a's and b's value point to the same memory
// which is why setting b to 20 also makes a's value 20
// and also why deleting a, then b leads to an attempted double
// free on the value pointer

// Ideally, this fix for this is to override the assignment
// operator to fix this; I'm not immediately sure how to do that,
// so I've gone the simpler route of constructing b with a's actual value.
// class Buffer {
//    private:
//     int* data;

//    public:
//     Buffer(int value) { data = new int(value); }

//     // This is a correct copy constructor definition
//     Buffer(const Buffer& other) { data = new int(*other.data); }

//     // Copy-assignment (b = a when b and a already exist)
//     Buffer& operator=(const Buffer& other) {
//         if (this != &other) {
//             *data = *other.data;
//         }

//         return *this;
//     }

//     ~Buffer() { delete data; }

//     void set(int value) { *data = value; }

//     int get() const { return *data; }
// };

// int main() {
//     Buffer a(10);
//     Buffer b = a;
//     // Buffer b(a.get());

//     b.set(20);

//     std::cout << "a: " << a.get() << "\n";
//     std::cout << "b: " << b.get() << "\n";

//     return 0;
// }

// I guess I'm a little less sure on this one.
// After learning the answer: you add brackets to delete
// when deleting an allocated array.
// I did not know that; probably a good thing to have caught though.
int* createArray(int size) {
    int* arr = new int[size];

    for (int i = 0; i < size; i++) {
        arr[i] = i * 10;
    }

    return arr;
}

int main() {
    int* values = createArray(5);

    std::cout << values[2] << "\n";

    // delete values;
    delete[] values;

    return 0;
}