#include <iostream>


// This is very simple int linked list class defintion.

// datatype to form structure of linked list
// We'll through in both a next and a previous for funsies
struct ListNode {
    ListNode* next;
    ListNode* prev;
    int val;

    // Constructor to initialize with value
    ListNode(int data) : next(nullptr), prev(nullptr), val(data) {};

    // Default, blank constructor
    ListNode() : next(nullptr), prev(nullptr), val(0) {};
};

class LinkedList {
   protected:
    ListNode* start;
    ListNode* end;
    int size;
    bool empty;

   public:
    // Default constructor
    LinkedList() : size(0), empty(true) {
        start = new ListNode();
        end = start;
    };

    // insert a new element at the end
    int insert(int val) {
        // Check if list is empty; if so, populate start node
        if (this->empty) {
            // Populate starter node
            this->start->val = val;
            this->empty = false;
        } else {
            ListNode* newnode = new ListNode(val);
            this->end->next = newnode;
            newnode->prev = this->end;
            this->end = newnode;
        }
        size++;
        return 0;
    }

    // Returns the number of items in the list
    int length() { return this->size; }

    // Inserts a new element as the kth item in the list
    // int insert(int& val, int k){
    //     // Element that exists before kth
    //     kprev

    // }

    // attempts to grab k-1th element in list
    // intention: array-like indexing
    int at(int k) {
        ListNode* current = this->start;
        int count = 0;
        while (current->next != nullptr && count < k) {
            current = current->next;
            count++;
        }
        // check that we actually have something
        if (current != nullptr) {
            return current->val;
        } else {
            // super basic exception
            throw 404;
        }
    }
};

// << Overload for linkedList
// Ideally this uses iterators or whatever instead of ll.at but fine for now
std::ostream& operator<<(std::ostream& os, LinkedList& ll) {
    os << "( ";

    int size = ll.length();

    for (int i = 0; i < size; i++) {
        os << ll.at(i) << " ";
        if (i < size - 1) {
            os << "<-> ";
        } else {
            os << ")";
        }
    }

    return os;
}

// Simple main to test out my implementation
int main(int argc, char* argv[]) {
    // Create a new linkedList
    LinkedList ll = LinkedList();

    ll.insert(1);
    ll.insert(2);
    ll.insert(3);

    std::cout << ll << "\n";



    return 0;
}