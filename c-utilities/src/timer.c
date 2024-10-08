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

void print_human_readable_stats(const Timer *timer, long number_of_inferences){
    if(timer->end == -1){
        printf("Warning: Timer has not been stopped yet.\n");
        return;
    }
    long elapsed_time = timer->elapsed_time;
    long hours = elapsed_time / 3600000000;
    elapsed_time -= hours * 3600000000;
    long minutes = elapsed_time / 60000000;
    elapsed_time -= minutes * 60000000;
    long seconds = elapsed_time / 1000000;
    elapsed_time -= seconds * 1000000;
    long milliseconds = elapsed_time / 1000;
    elapsed_time -= milliseconds * 1000;
    long microseconds = elapsed_time;
    printf("\n\n----------------------------------------------------------------\n");
    printf("Benchmark results:\n");
    printf("Elapsed time: %ldh %ldm %lds %ldms %ldus\n", hours, minutes, seconds, milliseconds, microseconds);
    if(number_of_inferences > 0) {
        float avg_latency_ms = (float)timer->elapsed_time / number_of_inferences / 1000;
        printf("Average time per inference: %f ms\n", avg_latency_ms);
        printf("Average FPS rate: %f\n", 1000.0f / avg_latency_ms);
    }
    printf("----------------------------------------------------------------\n\n");
}

static long get_current_us(){
    struct timeval time;
    gettimeofday(&time, NULL);
    long total_microseconds = time.tv_sec * 1000000 + time.tv_usec;
    return total_microseconds;
}