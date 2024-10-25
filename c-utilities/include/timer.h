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
 * Prints the elapsed time in a human-readable format.
 *
 * @note if the timer has not been stopped yet, a warning message will be printed.
 * @note if the number of inferences is greater than 0, the average time per inference and the average FPS rate will be printed.
 * 
 * @param timer The timer object to be printed.
 * @param number_of_inferences The number of inferences to calculate the average time per inference.
 */
void print_human_readable_stats(const Timer *timer, long number_of_inferences);


#endif // TIMER