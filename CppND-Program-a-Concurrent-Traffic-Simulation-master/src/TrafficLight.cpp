#include <iostream>
#include <random>
#include <chrono>
#include "TrafficLight.h"

// MessageQueue implementation

template <typename T>
void MessageQueue<T>::send(T &&msg) {
    std::unique_lock<std::mutex> lock(_mutex);
    _queue.emplace_back(std::move(msg));
    lock.unlock();
    _condition.notify_one();
}

template <typename T>
T MessageQueue<T>::receive() {
    std::unique_lock<std::mutex> lock(_mutex);
    _condition.wait(lock, [this] { return !_queue.empty(); });
    T phase = std::move(_queue.back());
    _queue.pop_back();
    return phase;
}

// TrafficLight implementation

TrafficLight::TrafficLight() {
    _currentPhase = TrafficLightPhase::red;
}

TrafficLight::~TrafficLight() {
    // Destructor to join all threads before destruction
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase() {
    return _currentPhase;
}

void TrafficLight::waitForGreen() {
    while (true) {
        TrafficLightPhase phase = _queue.receive();
        if (phase == TrafficLightPhase::green) {
            return;
        }
    }
}

void TrafficLight::simulate() {
    // Start the thread to cycle through traffic light phases
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

void TrafficLight::ToggleTrafficLightPhase() {
    if (_currentPhase == TrafficLightPhase::green) {
        _currentPhase = TrafficLightPhase::red;
    } else if (_currentPhase == TrafficLightPhase::red) {
        _currentPhase = TrafficLightPhase::green;
    }
}

int TrafficLight::getRandomCycleDuration() {
    std::random_device rand;
    std::mt19937 gen(rand());
    std::uniform_int_distribution<int> dis(4000, 6000);
    return dis(gen);
}

void TrafficLight::cycleThroughPhases() {
    int cycleDuration = getRandomCycleDuration();
    auto startTime = std::chrono::steady_clock::now();
    while (true) {
        // Measure the time between two loop cycles
        long timeDifference = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - startTime
        ).count();

        if (timeDifference >= cycleDuration) {
            // Toggle the current phase of the traffic light between red and green
            ToggleTrafficLightPhase();

            _queue.send(std::move(_currentPhase));

            startTime = std::chrono::steady_clock::now();
            cycleDuration = getRandomCycleDuration();
        }
        // Calculate a random cycle duration between 4 and 6 seconds

        // Wait for 1 ms between two cycles
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
