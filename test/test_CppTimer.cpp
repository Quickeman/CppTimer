#include "CppTimer.hpp"

#include "unit_test_framework.h"

class TestCallback : public ClockCallback {
public:
    TestCallback() { count = 0; }
    ~TestCallback() = default;

    void onTick() override {
        count++;
    }

    int count;
};

int main() {
    Clock clock;
    Timer timer;
    TestCallback timer_cb;
    int clk_count = 0;

    test::check(clk_count, 0);
    test::check(timer_cb.count, 0);

    timer.start(std::chrono::milliseconds(50), timer_cb);
    test::check(timer_cb.count, 0, 0, "Timer, just started");
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    test::check(timer_cb.count, 1, 0, "Timer, after 60ms");
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    test::check(timer_cb.count, 1, 0, "Timer, after 120ms");

    clock.start(std::chrono::milliseconds(50), [&clk_count](){ clk_count++; });
    test::check(clk_count, 0, 0, "Clock, just started");
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    test::check(clk_count, 1, 0, "Clock, after 60ms");
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    test::check(clk_count, 2, 0, "Clock, after 120ms");
    clock.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    test::check(clk_count, 2, 0, "Clock, 180ms after start, 60ms after stop");
}
