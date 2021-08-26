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
    Stopwatch_ms swatch;
    Clock clock;
    Timer timer;

    vector<milliseconds> swt{milliseconds(10), milliseconds(20)};
    int clk_count = 0;
    TestCallback timer_cb;

    test::check(swatch.retrieve().empty());
    test::check(clk_count, 0);
    test::check(timer_cb.count, 0);

    swatch.start();
    test::check(swatch.elapsed() == milliseconds(0), "Stopwatch, just started");
    std::this_thread::sleep_for(swt[0]);
    swatch.record();
    test::check(swatch.elapsed() >= swt[0], "Stopwatch, after 10ms");
    std::this_thread::sleep_for(swt[1]);
    swatch.record();
    test::check(swatch.elapsed() >= (swt[0] + swt[1]), "Stopwatch, after 30ms");
    auto v = swatch.retrieve();
    test::check(v[0] >= swt[0], "Stopwatch, comparing records");
    test::check(v[1] >= (swt[0] + swt[1]), "Stopwatch, comparing records");

    timer.start(milliseconds(50), timer_cb);
    test::check(timer_cb.count, 0, 0, "Timer, just started");
    std::this_thread::sleep_for(milliseconds(60));
    test::check(timer_cb.count, 1, 0, "Timer, after 60ms");
    std::this_thread::sleep_for(milliseconds(60));
    test::check(timer_cb.count, 1, 0, "Timer, after 120ms");

    clock.start(milliseconds(50), [&clk_count](){ clk_count++; });
    test::check(clk_count, 0, 0, "Clock, just started");
    std::this_thread::sleep_for(milliseconds(60));
    test::check(clk_count, 1, 0, "Clock, after 60ms");
    std::this_thread::sleep_for(milliseconds(60));
    test::check(clk_count, 2, 0, "Clock, after 120ms");
    clock.stop();
    std::this_thread::sleep_for(milliseconds(60));
    test::check(clk_count, 2, 0, "Clock, 180ms after start, 60ms after stop");
}
