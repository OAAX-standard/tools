#include "memory.h"
#include <sys/time.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static long get_current_memory_kb();
static long get_current_ms();


Memory *create_memory_records(int capacity, float interval){
    Memory *memory = malloc(sizeof(Memory));
    memory->length = 0;
    memory->first_value = -1;
    memory->last_timestamp = 0;
    memory->capacity = capacity;
    memory->interval = (int) (interval * 1000.0f); 
    memory->array = (int *) calloc(capacity, sizeof(int));
    return memory;
}

void record_memory(Memory *memory){
    // Stop recording when reaching the recorder's capacity
    // if(memory->length == memory->capacity) return;

    long ts = get_current_ms();
    // check if we're good to record a new value
    if(ts - memory->last_timestamp < memory->interval)
        return;
    memory->last_timestamp = ts;
    // get memory value
    long value = get_current_memory_kb();
    int index = memory->length;
    // Remember the first recorded value
    if (index == 0)
        memory->first_value = value;
    // Record the value
    index = index % memory->capacity;
    memory->array[index] = value;
    memory->length++;
}

int get_number_records(Memory *memory){
    int length  = memory->length;
    if (memory->length > memory->capacity) length = memory->capacity;
    return length;
}

int get_first_record(Memory *memory){
    return memory->first_value;
}

int get_last_record(Memory *memory){
    if(memory->length == 0) return -1;
    int index = (memory->length - 1) % memory->capacity;
    return memory->array[index];
}

int get_record(Memory *memory, int index){
    // The index should be in range [0, length or capacity)
    if(memory->length <= index || memory->capacity <= index) return -1;
    // check if we've cycled back to the front of the array
    if (memory->length > memory->capacity){
        index += (memory->length % memory->capacity);
        index = index % memory->capacity;
    }
    return memory->array[index];
}

void cleanup_memory_records(Memory *memory){
    free(memory->array);
    free(memory);
}

bool is_there_leak(Memory *memory){
    long first = get_first_record(memory);
    long last = get_last_record(memory);
    // check if there's a 1% increase in memory
    return last > first * 1.01f;
}

void save_to_file(Memory *memory){
    FILE* file = fopen("memory.csv", "w");

    int length = get_number_records(memory);
    fprintf(file, "%d\n", memory->first_value); // Append first value to the file
    for(int i=0; i<length; i++)
        fprintf(file, "%d\n", get_record(memory, i)); // Append the new value to the file
    
    fclose(file);
}

static long get_current_memory_kb(){
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    return r_usage.ru_maxrss * 1024;
}

static long get_current_ms(){
    struct timeval time;
    gettimeofday(&time, NULL);
    long total_ms = time.tv_sec * 1000 + time.tv_usec / 1000;
    return total_ms;
}