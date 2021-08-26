#ifndef _CPPTIMER_H
#define _CPPTIMER_H

#include <chrono>
#include <thread>
#include <variant>
#include <vector>

/** Callback class used to define action taken on a clock tick. */
class ClockCallback {
public:
    /** Default constructor. */
    ClockCallback() = default;
    /** Default destructor. */
    ~ClockCallback() = default;

    /** Method called on each clock tick.
     * Override to add functionality.
     */
    virtual void onTick() = 0;
};


/** Time device class for timing system. */
class _TimeDevice {
public:
    /** Default constructor. */
    _TimeDevice() = default;

    /** Default destructor. */
    ~_TimeDevice() = default;

protected:
    /** Clock type to use. */
    typedef std::chrono::steady_clock Clock_t;

    /** Duration type to use for internal mechanics. */
    typedef std::chrono::nanoseconds duration_t;

    /** Clock used to measure time. */
    Clock_t clock;

    /** Start time to measure from. */
    std::chrono::time_point<Clock_t> startTime;
};


/** Stopwatch
 * Measures the time elapsed between two events.
 * Essentially a wrapper around a C++ STL clock-type object.
 * @param TimeUnit Unit of time to use; a std::chrono::duration type.
 */
template<class TimeUnit>
class Stopwatch : public _TimeDevice {
public:
    /** Constructor. */
    Stopwatch() {
        start();
    }

    /** Default destructor. */
    ~Stopwatch() = default;

    /** Starts the Stopwatch. */
    inline void start() {
        startTime = clock.now();
    }

    /** @return time since construction or the last call to @ref start. */
    inline TimeUnit elapsed() {
        return std::chrono::duration_cast<TimeUnit>(clock.now() - startTime);
    }

    /** Records the elapsed time.
     * Retrieve the list of elapsed times with @ref retrieve.
     */
    inline void record() {
        times.push_back(elapsed());
    }

    /** Retrieves the times recorded with @ref record. */
    inline std::vector<TimeUnit>& retrieve() {
        return times;
    }

private:
    /** Vector of recorded elapsed times. */
    std::vector<TimeUnit> times;
};

typedef Stopwatch<std::chrono::seconds> Stopwatch_s;
typedef Stopwatch<std::chrono::milliseconds> Stopwatch_ms;
typedef Stopwatch<std::chrono::microseconds> Stopwatch_us;
typedef Stopwatch<std::chrono::nanoseconds> Stopwatch_ns;


/** Trigger device.
 * Time-based event triggering class.
 */
class _EventTriggerDevice : public _TimeDevice {
public:
    /** Constructor. */
    _EventTriggerDevice();

    /** Disable copy constructor. */
    _EventTriggerDevice(const _EventTriggerDevice&) = delete;
    /** Disable copy assignment operator. */
    _EventTriggerDevice& operator=(const _EventTriggerDevice&) = delete;

    /** Move constructor. */
    _EventTriggerDevice(_EventTriggerDevice&& td);
    /** Move assignment operator. */
    _EventTriggerDevice& operator=(_EventTriggerDevice&& td);

    /** Destructor.
     * Stops the clock if currently running.
     */
    ~_EventTriggerDevice();

    /** Starts the clock with the given period.
     * @param period Tick period as a std::chrono::duration<>, e.g. std::chrono::milliseconds.
     * @param callback A @ref ClockCallback object to be used on tick events.
     */
    template<class R, class P>
    void start(std::chrono::duration<R, P> period, ClockCallback& callback) {
        this->period = std::chrono::duration_cast<duration_t>(period);
        this->callback = &callback;
        _start();
    }

    /** Starts the clock with the given period.
     * @param period Tick period as a std::chrono::duration<>, e.g. std::chrono::milliseconds.
     * @param callback A std::function<void()> to be called on tick events.
     */
    template<class R, class P>
    void start(std::chrono::duration<R, P> period, std::function<void()> callback) {
        this->period = std::chrono::duration_cast<duration_t>(period);
        this->callback = callback;
        _start();
    }

    /** Stops the clock.
     * Can be restarted with @ref start.
     */
    void stop();

protected:
    /** Determines internal actions that should be performed on each tick. */
    virtual void onTick() = 0;

    /** Thread of execution to measure elapsed time. */
    std::thread timeThread;

    /** Clock running flag. */
    bool running;

private:
    /** Starts the clock. */
    void _start();

    /** Method given to the timing thread.
     * Measures the current time to see if the @ref period has passed. If it
     * has, calls the @ref ClockCallback method @ref onTick.
     * @param self Reference to `this`.
     */
    static void run(_EventTriggerDevice* self);

    /** Method called by @ref run when @ref period has passed.
     * Calls @ref ClockCallback::onTick, then @ref onTick.
     */
    void tickEvent();

    /** Time period to measure for in @ref run. */
    duration_t period;

    /** Variant storing the callback object to use. */
    std::variant<ClockCallback*, std::function<void()>> callback;
};


/** Clock.
 * Ticks after the given time, then at that interval.
 */
class Clock : public _EventTriggerDevice {
public:
    Clock() = default;
    ~Clock() = default;

protected:
    void onTick() override;
};


/** Timer.
 * Ticks once after the given time.
 */
class Timer : public _EventTriggerDevice {
public:
    Timer() = default;
    ~Timer() = default;

protected:
    void onTick() override;
};

#endif
