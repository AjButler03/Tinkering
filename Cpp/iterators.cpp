// 10/01/2026
// I haven't really done much with iterators, so here I'm just going to play around with them a bit.

#include <vector>
#include <iostream>

int main (int argc, char* argv[]) {
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::vector<int>::iterator it = nums.begin();

    it += 15;

    // it--;
    std::cout << *(it) << "\n";
    std::cout << (it > nums.end()) << "\n";

    // for (it = nums.begin(); it != nums.end(); ++it){
    //     std::cout << *it << "\n";
    // }

    return 0;
}