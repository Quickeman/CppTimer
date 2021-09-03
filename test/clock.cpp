#include "CppTimer.hpp"

#include "unit_test_framework.h"

using namespace std;
using namespace std::chrono;

int main() {
    Clock clock;
    int clk_count = 0;
    test::check(clk_count, 0);

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