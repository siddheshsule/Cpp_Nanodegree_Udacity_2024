#include<iostream>
#include<thread>
#include<future>
#include <string>


void fun(std::promise<std::string> && pr) {
    std::cout << "Worker sending..." << std::endl;
    pr.set_value("Message from thread");
}



int main() {
    std::promise<std::string> pr;
    std::future<std::string> fut = pr.get_future();
    std::thread th(fun, std::move(pr));
    std::cout << "Main receiving:" << std::endl;
    std::string str = fut.get();
    std::cout << str << std::endl;
    th.join();
}