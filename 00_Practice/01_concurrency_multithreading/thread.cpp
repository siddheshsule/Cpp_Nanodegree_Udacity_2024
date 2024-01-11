#include <iostream>
#include <thread>

void threadFunc() {
    std::cout << "Hello from thread!" << std::endl;
}

int main() {
    std::thread t(threadFunc);
    std::cout << "Hello from main function!" << std::endl;
    t.join();
}