#include <string>
#include <vector>
#include <iostream>
#include <thread>

void repeat(std::string str, int n) {
    if (n > 0) {
        str [0] = '*';
        std::cout << str << std::endl;
        repeat(str, n-1);
    }
}

int main () {
    std::string str{"Hello"};
    std::thread t(repeat, str, 7);
    t.join();
    std::cout << "After join:" << str << std::endl;
}