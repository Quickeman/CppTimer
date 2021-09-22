#include "CppTimer.hpp"

using namespace std;

// class _EventTriggerDevice

_EventTriggerDevice::_EventTriggerDevice() {
    running = false;
}

_EventTriggerDevice::_EventTriggerDevice(_EventTriggerDevice&& td):
timeThread(move(td.timeThread)) {
    running = false;
}

_EventTriggerDevice& _EventTriggerDevice::operator=(_EventTriggerDevice&& td) {
    if (running)
        stop();
    if (timeThread.joinable())
        timeThread.join();
    timeThread = move(td.timeThread);
    return *this;
}

_EventTriggerDevice::~_EventTriggerDevice() {
    if (running)
        stop();
    if (timeThread.joinable())
        timeThread.join();
}

void _EventTriggerDevice::stop() {
    running = false;
}

void _EventTriggerDevice::_start() {
    startTimeMutex.lock();
    startTime = clock.now();
    startTimeMutex.unlock();

    if (timeThread.joinable()) {
        running = false;
        timeThread.join();
    }
    running = true;

    timeThread = thread([this]() { this->run(); });
}

void _EventTriggerDevice::run() {
    Clock_t::time_point currTime;
    duration_t elapsed;

    while (running) {
        currTime = clock.now();

        startTimeMutex.lock();
        auto st = startTime;
        startTimeMutex.unlock();

        elapsed = chrono::duration_cast<duration_t>(currTime - st);

        if (elapsed >= period) {
            tickEvent();
        }
    }
}

void _EventTriggerDevice::tickEvent() {
    if (holds_alternative<ClockCallback*>(callback)) {
        get<ClockCallback*>(callback)->onTick();
    }
    else if (holds_alternative<function<void()>>(callback)) {
        get<function<void()>>(callback)();
    }
    onTick();
}


// class Clock

void Clock::onTick() {
    lock_guard<mutex> lg(startTimeMutex);
    startTime = clock.now();
}


// class Timer

void Timer::onTick() {
    stop();
}
