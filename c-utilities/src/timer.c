#include "timer.h"

static long get_current_us();

void start_recording(Timer *timer){
    timer->start = get_current_us();
    timer->end = -1;
    timer->elapsed_time = -1;
}

void stop_recording(Timer *timer){
    timer->end = get_current_us();
    timer->elapsed_time = timer->end - timer->start;
}

long get_elapsed_time_us(const Timer *timer){
    return timer->elapsed_time;
}

long get_elapsed_time_ms(const Timer *timer){
    return timer->elapsed_time / 1000;
}

float get_elapsed_time_s(const Timer *timer){
    return timer->elapsed_time / 1000000.0f;
}

static long get_current_us(){
    struct timeval time;
    gettimeofday(&time, NULL);
    long total_microseconds = time.tv_sec * 1000000 + time.tv_usec;
    return total_microseconds;
}