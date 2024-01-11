#include <iostream>
#include <future>


struct Counter {
    Counter (int n) : k(n) {};
    int k;
};

struct Holder {
    Holder (int n) : c(new Counter(n)) {};
    Counter* c;
};


int main() {
    const Holder h(10);
    std::future<void> fut = std::async([](Holder const & h) 
    {
        ++(h.c->k);
    }, std::ref(h));
    std::cout << h.c->k << std::endl;
    fut.wait();
    std::cout << h.c->k << std::endl;
}