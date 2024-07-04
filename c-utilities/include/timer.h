#ifndef TIMER
#define TIMER

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <locale.h>

typedef struct {
    long start;         // Timestamp when the timer starts (in microseconds)
    long end;           // Timestamp when the timer stops (in microseconds)
    long elapsed_time;  // Total elapsed time between start and end (in microseconds)
} Timer;


/**
 * Starts the timer.
 *
 * @param timer The timer object to be started.
 */
void start_recording(Timer* timer);

/**
 * Stops the timer and calculates the elapsed time.
 *
 * @param timer The timer object to be stopped.
 */
void stop_recording(Timer* timer);

/**
 * Returns the elapsed time in microseconds.
 *
 * @param timer The timer object.
 * @return Elapsed time in microseconds.
 */
long get_elapsed_time_us(const Timer* timer);

/**
 * Returns the elapsed time in milliseconds.
 *
 * @param timer The timer object.
 * @return Elapsed time in milliseconds.
 */
long get_elapsed_time_ms(const Timer* timer);

/**
 * Returns the elapsed time in seconds.
 *
 * @param timer The timer object.
 * @return Elapsed time in seconds.
 */
float get_elapsed_time_s(const Timer* timer);


#endif // TIMER