#include "CppTimer.hpp"

#include "unit_test_framework.h"

using namespace std;
using namespace std::chrono;

int main() {
    Clock clock1;
    int clk_count = 0;
    test::check(clk_count, 0);

    auto clocker = [&clk_count](){ clk_count++; };

    clock1.start(milliseconds(20), clocker);
    test::check(clk_count, 0, 0, "Clock, just started");
    std::this_thread::sleep_for(milliseconds(22));
    test::check(clk_count, 1, 0, "Clock, after 22ms");
    std::this_thread::sleep_for(milliseconds(22));
    test::check(clk_count, 2, 0, "Clock, after 44ms");
    clock1.stop();
    std::this_thread::sleep_for(milliseconds(22));
    test::check(clk_count, 2, 0, "Clock, 66ms after start, 22ms after stop");

    // Test re-starting with different period
    Clock clock2;
    clk_count = 0;
    clock2.start(milliseconds(20), clocker);
    test::check(clk_count, 0, 0, "Clock re-start, just started.");
    std::this_thread::sleep_for(milliseconds(22));
    test::check(clk_count, 1, 0, "Clock re-start, after 22ms");
    clock2.start(milliseconds(10), clocker);
    test::check(clk_count, 1, 0, "Clock re-start, just after re-starting.");
    std::this_thread::sleep_for(milliseconds(22));
    test::check(clk_count, 3, 0, "Clock re-start, after 22ms.");
    clock2.stop();
    std::this_thread::sleep_for(milliseconds(30));
    test::check(clk_count, 3, 0, "Clock, 52ms after start, 30ms after stop");
}