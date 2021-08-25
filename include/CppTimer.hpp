#ifndef _CPPTIMER_H
#define _CPPTIMER_H

#include <chrono>
#include <thread>

/** Callback class used to define action taken on a @ref _Clock tick. */
class ClockCallback {
public:
    /** Default constructor. */
    ClockCallback() = default;
    /** Default destructor. */
    ~ClockCallback() = default;

    /** Method called on each clock tick.
     * Override to add functionality.
     * @param arg User-defined argument given to the @ref _TimeDevice
     */
    virtual void onTick() = 0;
};


/** Time device class for timing system. */
class _TimeDevice {
public:
    /** Clock type to use. */
    typedef std::chrono::steady_clock Clock_t;

    /** Duration type to use for internal mechanics. */
    typedef std::chrono::microseconds Time_t;

    /** Constructor. */
    _TimeDevice();

    /** Destructor.
     * Stops the clock if currently running.
     */
    ~_TimeDevice();

    /** Starts the clock with the given period.
     * Converts the period to std::chrono::microseconds, then calls @ref start_us.
     * @param period Tick period as a std::chrono::duration.
     * @param callback A @ref ClockCallback object to be used on tick events.
     * @note The internal timing system uses microseconds (us), so nanosecond
     * precision is not yet supported.
     */
    template<class R, class P>
    void start(std::chrono::duration<R, P> period, ClockCallback& callback) {
        start_us(std::chrono::duration_cast<Time_t>(period), callback);
    }

    /** Starts the clock with the given period.
     * @param period Tick period in std::chrono::microsecond units.
     * @param callback A @ref ClockCallback object to be used on tick events.
     */
    void start_us(Time_t period, ClockCallback& callback);

    /** Stops the clock.
     * Can be restarted with @ref start_ms or @ref start_us.
     */
    void stop();

protected:
    /** Determines . */
    virtual void onTick() = 0;

    /** Thread of execution to measure elapsed time. */
    std::thread timeThread;

    /** Clock used to measure time. */
    Clock_t clock;

    /** Start time to measure from. */
    std::chrono::time_point<Clock_t> startTime;

    /** Clock running flag. */
    bool running;

private:
    /** Method given to the timing thread.
     * Measures the current time to see if the @ref period has passed. If it
     * has, calls the @ref ClockCallback method @ref onTick.
     * @param self Reference to `this`.
     */
    static void run(_TimeDevice* self);

    /** Method called by @ref run when @ref period has passed.
     * Calls @ref ClockCallback::onTick, then @ref onTick.
     */
    void tickEvent();

    /** Time period to measure for in @ref run. */
    Time_t period;

    /** Pointer to the callback object to use. */
    ClockCallback* callback;
};


/** Clock.
 * Ticks after the given time, then at that interval.
 */
class Clock : public _TimeDevice {
public:
    Clock() = default;
    ~Clock() = default;

protected:
    void onTick() override;
};


/** Timer.
 * Ticks once after the given time.
 */
class Timer : public _TimeDevice {
public:
    Timer() = default;
    ~Timer() = default;

protected:
    void onTick() override;
};

#endif
