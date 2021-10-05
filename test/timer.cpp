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

    timer.start(milliseconds(20), timer_cb);
    test::check(timer_cb.count, 0, 0, "Timer, just started");
    std::this_thread::sleep_for(milliseconds(22));
    test::check(timer_cb.count, 1, 0, "Timer, after 22ms");
    std::this_thread::sleep_for(milliseconds(22));
    test::check(timer_cb.count, 1, 0, "Timer, after 44ms");

    // Test for re-using the Timer
    timer.start(milliseconds(20), timer_cb);
    test::check(timer_cb.count, 1, 0, "Timer repeat, just started");
    std::this_thread::sleep_for(milliseconds(22));
    test::check(timer_cb.count, 2, 0, "Timer repeat, after 22ms");
    std::this_thread::sleep_for(milliseconds(22));
    test::check(timer_cb.count, 2, 0, "Timer repeat, after 44ms");

    // Test for re-using the Timer with stop()
    timer.start(milliseconds(20), timer_cb);
    test::check(timer_cb.count, 2, 0, "Timer stop, just started");
    std::this_thread::sleep_for(milliseconds(5));
    timer.stop();
    test::check(timer_cb.count, 2, 0, "Timer stop, after 5ms");
    std::this_thread::sleep_for(milliseconds(25));
    test::check(timer_cb.count, 2, 0, "Timer stop, after 30ms");

    // Test for re-using the Timer
    timer.start(milliseconds(20), timer_cb);
    test::check(timer_cb.count, 2, 0, "Timer restarted, just started");
    std::this_thread::sleep_for(milliseconds(22));
    test::check(timer_cb.count, 3, 0, "Timer restarted, after 22ms");
    std::this_thread::sleep_for(milliseconds(22));
    test::check(timer_cb.count, 3, 0, "Timer restarted, after 44ms");
}
