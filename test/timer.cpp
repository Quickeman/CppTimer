#include "CppTimer.hpp"

#include "unit_test_framework.h"

using namespace std;
using namespace std::chrono;

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
    Timer timer;
    TestCallback timer_cb;
    test::check(timer_cb.count, 0);

    timer.start(milliseconds(50), timer_cb);
    test::check(timer_cb.count, 0, 0, "Timer, just started");
    std::this_thread::sleep_for(milliseconds(60));
    test::check(timer_cb.count, 1, 0, "Timer, after 60ms");
    std::this_thread::sleep_for(milliseconds(60));
    test::check(timer_cb.count, 1, 0, "Timer, after 120ms");

    // Test for re-using the Timer
    timer.start(milliseconds(50), timer_cb);
    test::check(timer_cb.count, 1, 0, "Timer repeat, just started");
    std::this_thread::sleep_for(milliseconds(60));
    test::check(timer_cb.count, 2, 0, "Timer repeat, after 60ms");
    std::this_thread::sleep_for(milliseconds(60));
    test::check(timer_cb.count, 2, 0, "Timer repeat, after 120ms");

    // Test for re-using the Timer with stop()
    timer.start(milliseconds(50), timer_cb);
    test::check(timer_cb.count, 2, 0, "Timer stop, just started");
    std::this_thread::sleep_for(milliseconds(10));
    timer.stop();
    test::check(timer_cb.count, 2, 0, "Timer stop, after 10ms");
    std::this_thread::sleep_for(milliseconds(60));
    test::check(timer_cb.count, 2, 0, "Timer stop, after 70ms");

    // Test for re-using the Timer
    timer.start(milliseconds(50), timer_cb);
    test::check(timer_cb.count, 2, 0, "Timer restarted, just started");
    std::this_thread::sleep_for(milliseconds(60));
    test::check(timer_cb.count, 3, 0, "Timer restarted, after 60ms");
    std::this_thread::sleep_for(milliseconds(60));
    test::check(timer_cb.count, 3, 0, "Timer restarted, after 120ms");
}
