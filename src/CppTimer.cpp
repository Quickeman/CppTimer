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

void _TimeDevice::start_us(Time_t period, ClockCallback& callback) {
    startTime = clock.now();

    this->period = period;
    this->callback = &callback;

    running = true;

    timeThread = std::thread(_TimeDevice::run, this);
}

void _TimeDevice::stop() {
    running = false;
}

void _TimeDevice::run(_TimeDevice* self) {
    Clock_t::time_point currTime;
    Time_t elapsed;

    while (self->running) {
        currTime = self->clock.now();
        elapsed = std::chrono::duration_cast<Time_t>(currTime - self->startTime);

        if (elapsed >= self->period) {
            self->tickEvent();
        }
    }
}

void _TimeDevice::tickEvent() {
    callback->onTick();
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
