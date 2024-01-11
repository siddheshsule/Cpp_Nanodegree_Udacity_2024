#include<iostream>
#include<vector>
#include<thread>
#include<chrono>
//#include <format>

int main() {
    std::vector<std::thread> threads;
    for(int i = 0; i < 16; ++i) 
    {
        threads.emplace_back([&](){
            std::this_thread::sleep_for(std::chrono::milliseconds(i * 10));
            std::cout << "Hello from thread " << i << std::endl;
        });
    }
    std::cout << "Hello from main" << std::endl;
    for (auto &t : threads) {
        t.join();
    }
}
