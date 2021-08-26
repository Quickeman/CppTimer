#include "CppTimer.hpp"

using namespace std;

// class _EventTriggerDevice

_EventTriggerDevice::_EventTriggerDevice():
running(false) {

}

_EventTriggerDevice::_EventTriggerDevice(_EventTriggerDevice&& td):
running(false), timeThread(move(td.timeThread)) {

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
    startTime = clock.now();

    running = true;

    timeThread = thread(_EventTriggerDevice::run, this);
}

void _EventTriggerDevice::run(_EventTriggerDevice* self) {
    Clock_t::time_point currTime;
    duration_t elapsed;

    while (self->running) {
        currTime = self->clock.now();
        elapsed = chrono::duration_cast<duration_t>(currTime - self->startTime);

        if (elapsed >= self->period) {
            self->tickEvent();
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
    startTime = clock.now();
}


// class Timer

void Timer::onTick() {
    stop();
}
