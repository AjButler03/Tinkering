// Quick refresher on classes + inheritance
#include <iostream>
#include <string>

class vehicle1 {
   public:
    std::string make = "Kia";
    void hello() { std::cout << "Hello, I am a " << this->make << ".\n"; }
};

class car1 : public vehicle1 {
   public:
    std::string model = "Forte";
    void hello() {
        std::cout << "Hello, I am a " << this->make << " " << this->model
                  << ".\n";
    }
};

class vehicle2 {
   public:
    std::string make = "Kia";
    virtual void hello() {
        std::cout << "Hello, I am a " << this->make << ".\n";
    }

    // Similar to above: want virtual destructor so it can be overridden
    // by child classes, which may have more to deallocate.
    virtual ~vehicle2(){};
};

class car2 : public vehicle2 {
   public:
    std::string model = "Forte";
    void hello() override {
        std::cout << "Hello, I am a " << this->make << " " << this->model
                  << ".\n";
    }
};

int main(int argc, char* argv[]) {
    vehicle1 v1;
    car1 c1;
    vehicle1 list1[2] = {v1, c1};

    // This does not work as you might expect.
    list1[0].hello();
    list1[1].hello();

    vehicle2 v2;
    car2 c2;
    vehicle2 list2[2] = {v2, c2};

    list2[0].hello();
    list2[1].hello();  // should include 'forte', but does not.
    // Reason: This is directly storing the objects, meaning overridden
    // hello does not exist; only the original as part of the (now) vehicle2.

    // Utilizing and storing pointers fixes this problem
    vehicle2 *v3 = new vehicle2;
    car2 *c3 = new car2;
    vehicle2 *list3[2] = {v3, c3};
    list3[0]->hello();
    list3[1]->hello(); // Now this works.

}
