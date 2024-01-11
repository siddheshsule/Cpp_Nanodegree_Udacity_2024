#include<iostream>
#include<thread>
#include<chrono>


void func(int& x) {
    x++;
}


int main() {

    int i = 0;
    std::cout << "i: " << i << std::endl;
    std::thread t(func, std::ref(i));
    std::this_thread::sleep_for(std::chrono::milliseconds(i * 10));
    std::cout << "i: " << i << std::endl;
    t.join();
}