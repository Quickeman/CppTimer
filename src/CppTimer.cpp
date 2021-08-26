#include "CppTimer.hpp"

// class _TimeDevice

_TimeDevice::_TimeDevice():
running(false) {

}

_TimeDevice::_TimeDevice(_TimeDevice&& td):
running(false), timeThread(std::move(td.timeThread)) {

}

_TimeDevice& _TimeDevice::operator=(_TimeDevice&& td) {
    if (running)
        stop();
    if (timeThread.joinable())
        timeThread.join();
    timeThread = std::move(td.timeThread);
    return *this;
}

_TimeDevice::~_TimeDevice() {
    if (running)
        stop();
    if (timeThread.joinable())
        timeThread.join();
}

void _TimeDevice::stop() {
    running = false;
}

void _TimeDevice::_start() {
    startTime = clock.now();

    running = true;

    timeThread = std::thread(_TimeDevice::run, this);
}

void _TimeDevice::run(_TimeDevice* self) {
    Clock_t::time_point currTime;
    duration_t elapsed;

    while (self->running) {
        currTime = self->clock.now();
        elapsed = std::chrono::duration_cast<duration_t>(currTime - self->startTime);

        if (elapsed >= self->period) {
            self->tickEvent();
        }
    }
}

void _TimeDevice::tickEvent() {
    if (std::holds_alternative<ClockCallback*>(callback)) {
        std::get<ClockCallback*>(callback)->onTick();
    }
    else if (std::holds_alternative<std::function<void()>>(callback)) {
        std::get<std::function<void()>>(callback)();
    }
    onTick();
}


// class Clock

void Clock::onTick() {
    startTime = clock.now();
}


// class Timer

void Timer::onTick() {
    stop();
}
