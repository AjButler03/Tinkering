#include <iostream>
#include <set>
#include <vector>

using namespace std;

/*
 * This file is just to practice some basic implementations with C++.
 */

// Reverses an int vector array in place.
void reverseArray(vector<int>& arr) {
    // two index pointers; left/right
    int left = 0;
    int right = arr.size() - 1;

    // to swap values
    int temp;

    while (left < right) {
        temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
}

// Removes any duplicate values in array
void rmdups(vector<int>& arr) {
    // define set to store included values
    set<int> numset;

    // iterate through array, adding new entries and removing duplicate if
    // already in set
    for (int i = 0; i < arr.size(); i++) {
        if (numset.find(arr[i]) == numset.end()) {
            // not found, insert into the set
            numset.insert(arr[i]);
        } else {
            // remove element from vector
            arr.erase(arr.begin() + i);
            i--;  // fix i to account for shrinkage
        }
    }
}

// Finds minimum value in vector of ints
int findmin(vector<int>& arr) {
    int min = arr[0];
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

// Finds maximum value in vector of ints
int findmax(vector<int>& arr) {
    int max = arr[0];
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// prints out the elements of an int vector in order.
void printvector(vector<int>& arr) {
    for (const int& element : arr) {
        std::cout << element << " ";
    }
    std::cout << "\n";
}

int main(int argc, char* argv[]) {
    vector<int> arr = {2, 6, 8, 3, -5, 9, 1, 0, 12, 15, -2, 7, -2, 7};
    cout << "Array start: ";
    printvector(arr);
    reverseArray(arr);
    cout << "Array after: ";
    printvector(arr);
    cout << "Min: " << findmin(arr) << " Max: " << findmax(arr) << "\n";
    rmdups(arr);
    printvector(arr);
}