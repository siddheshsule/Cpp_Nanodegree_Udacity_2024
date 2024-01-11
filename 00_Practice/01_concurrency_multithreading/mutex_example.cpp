#include <iostream>
#include<thread>
#include <mutex>
#include<chrono>

std::mutex mtx;

void driveCar(std::string driver) {
    std::unique_lock<std::mutex> carLock(mtx);
    std::cout << driver << " is driving the car." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    std::cout << driver << " is finished driving the car." << std::endl;
    //carLock.unlock();
}

int main() {
    std::thread t1(driveCar, "Siddhesh");
    std::thread t2(driveCar, "Annemarie");
    t1.join();
    t2.join();
}