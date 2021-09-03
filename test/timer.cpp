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
}
