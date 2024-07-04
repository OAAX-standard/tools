#ifndef MEMORY_H
#define MEMORY_H

#include <sys/resource.h>
#include <stdbool.h>

typedef struct {
    int interval;           // in ms
    long last_timestamp;    // in ms
    int first_value;        // in Bytes
    int length;             // Number of times the memory usage has been recorded
    int capacity;           // Maximum number of records
    int *array;             // Array of memory usage records
} Memory;


/**
 * @brief Create a new Memory object
 * 
 * @param capacity The maximum number of records to store.
 * @param interval The interval in seconds between each record.
 * 
 * @return Memory* A pointer to the new Memory object.
 */
Memory *create_memory_records(int capacity, float interval);

/**
 * @brief Record the current memory usage.
 * 
 * @param memory A pointer to the Memory object.
 */
void record_memory(Memory *memory);

/**
 * @brief Get the number of records stored.
 * 
 * @param memory A pointer to the Memory object.
 * 
 * @return int The number of records stored.
 */
int get_number_records(Memory *memory);

/**
 * @brief Get the first record stored.
 * 
 * @param memory A pointer to the Memory object.
 * 
 * @return int The first record stored.
 */
int get_first_record(Memory *memory);

/**
 * @brief Get the last record stored.
 * 
 * @param memory A pointer to the Memory object.
 * 
 * @return int The last record stored.
 */
int get_last_record(Memory *memory);

/**
 * @brief Get the record at the specified index.
 * 
 * @param memory A pointer to the Memory object.
 * @param index The index of the record to get.
 * 
 * @return int The record at the specified index.
 */
int get_record(Memory *memory, int index);

/**
 * @brief Get the average memory usage.
 * 
 * It check if there's a 1% increase in memory usage between the first and last record.
 * 
 * @param memory A pointer to the Memory object.
 * 
 * @return float The average memory usage.
 */
bool is_there_leak(Memory *memory);

/**
 * @brief Save the memory records to a file.
 * It saves the records in a CSV file named in "./memory.csv".
 * 
 * @note The memory records can be visualized using this commad:
 *  `gnuplot -p -e "set term png; set output 'output.png'; plot '< cat memory.csv' using 1:xticlabels(1); set output"`
 * 
 * @warning The file is overwritten if it already exists.
 * 
 * @param memory A pointer to the Memory object.
 */
void save_to_file(Memory *memory);

/**
 * @brief Free the memory allocated for the Memory object.
 * 
 * @param memory A pointer to the Memory object.
 */
void cleanup_memory_records(Memory *memory);


#endif // MEMORY_H