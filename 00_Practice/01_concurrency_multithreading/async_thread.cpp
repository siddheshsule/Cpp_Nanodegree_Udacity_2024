#include<iostream>
#include<thread>
#include <string>
#include <future>


std::string fun(bool flag) {
    if (flag) {
        std::cout << "Worker returning..." << std::endl;
        return "Message from worker.";
    } else {
        std::cout << "Worker throwing..." << std::endl;
        throw std::runtime_error("Exception from thread thrown");
    }
}

int main() {
    std::future<std::string> fut = std::async(fun, false);
    std::cout << "Main receiving:" << std::endl;
    try {
        std::string str = fut.get();
        std::cout << str << std::endl;
    } catch(...) {
        std::cout << "Exception thrown" << std::endl;
    }
    
}