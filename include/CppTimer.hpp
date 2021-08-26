#ifndef _CPPTIMER_H
#define _CPPTIMER_H

#include <chrono>
#include <thread>
#include <variant>

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
    /** Constructor. */
    _TimeDevice();

    /** Disable copy constructor. */
    _TimeDevice(const _TimeDevice&) = delete;
    /** Disable copy assignment operator. */
    _TimeDevice& operator=(const _TimeDevice&) = delete;

    /** Move constructor. */
    _TimeDevice(_TimeDevice&& td);
    /** Move assignment operator. */
    _TimeDevice& operator=(_TimeDevice&& td);

    /** Destructor.
     * Stops the clock if currently running.
     */
    ~_TimeDevice();

    /** Starts the clock with the given period.
     * @param period Tick period as a std::chrono::duration<>, e.g. std::chrono::milliseconds.
     * @param callback A @ref ClockCallback object to be used on tick events.
     * @note The internal timing system uses microseconds (us), so nanosecond
     * precision is not yet supported.
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
     * @note The internal timing system uses microseconds (us), so nanosecond
     * precision is not yet supported.
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
    /** Clock type to use. */
    typedef std::chrono::steady_clock Clock_t;

    /** Duration type to use for internal mechanics. */
    typedef std::chrono::nanoseconds duration_t;

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
    /** Starts the clock. */
    void _start();

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
    duration_t period;

    /** Variant storing the callback object to use. */
    std::variant<ClockCallback*, std::function<void()>> callback;
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
