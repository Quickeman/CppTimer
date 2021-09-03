#include "CppTimer.hpp"

#include "unit_test_framework.h"

using namespace std;
using namespace std::chrono;

int main() {
    Stopwatch_ms swatch;
    vector<intmax_t> swt{10, 20};
    test::check(swatch.retrieve().empty());

    swatch.start();
    // Compare as ints
    test::check(swatch.elapsed(), intmax_t(0));
    std::this_thread::sleep_for(milliseconds(swt[0]));
    swatch.record();
    // Use duration<>.count() method to compare as ints
    test::check(swatch.elapsed() >= swt[0]);
    std::this_thread::sleep_for(milliseconds(swt[1]));
    swatch.record();
    // Compare duration<> objects directly
    test::check(swatch.elapsed_dur() >= milliseconds(swt[0] + swt[1]), "Stopwatch, after 30ms");
    auto v = swatch.retrieve();
    test::check(v[0] >= swt[0], "Stopwatch, comparing records");
    test::check(v[1] >= (swt[0] + swt[1]), "Stopwatch, comparing records");
}