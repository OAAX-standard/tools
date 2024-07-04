#include "memory.h"
#include "timer.h"

#define NUMBER_OR_INFERENCE 1000

int main(){

    Memory *memory = create_memory_records(10000, 0.01);
    Timer timer;

    start_recording(&timer);
    for (size_t i = 0; i < NUMBER_OR_INFERENCE; i++){
        record_memory(memory);
        char *tmp = (char *) malloc(1 * 1024 * sizeof(char)); // 10 KB 
        tmp[0] = 'a';
        for(size_t j = 1; j < 1 * 1024; j++){
            tmp[j] = tmp[j-1] + 1;
        }
        // Mimicking a memory leak
        free(tmp);
    }
    stop_recording(&timer);

    // Read statistics
    long first = get_first_record(memory);
    long last = get_last_record(memory);
    bool leaking = is_there_leak(memory);
    long elapsed_time_ms = get_elapsed_time_ms(&timer);

    printf("First memory record (B): %ld, last memory record (B): %ld, leaking: %d\n", first, last, leaking);
    printf("Memory difference: %ld, difference per inference run: %ld\n", last - first, (last - first) / NUMBER_OR_INFERENCE);
    printf("Elapsed time: %li ms, Latency: %.2f, Throughput: %.2f\n", elapsed_time_ms, 
            (1.0f * elapsed_time_ms) / NUMBER_OR_INFERENCE, 
            (1000.0f * NUMBER_OR_INFERENCE) / elapsed_time_ms );

    // clean up
    cleanup_memory_records(memory);

    return 0;
}