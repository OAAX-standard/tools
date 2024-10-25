#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

const char *human_memory_size(uint64_t bytes) {
    static char result[20]; // Use a static buffer to avoid multiple allocations
    const char *sizeNames[] = { "B", "KB", "MB", "GB", "TB" }; // Added TB for larger sizes

    if (bytes == 0) {
        snprintf(result, sizeof(result), "0 B");
        return result;
    }

    long i = (long)floor(log(bytes) / log(1024));
    if (i >= 4) i = 4; // Prevents overflow if sizes go beyond TB
    double humanSize = bytes / pow(1024, i);
    snprintf(result, sizeof(result), "%.2f %s", humanSize, sizeNames[i]);

    return result;
}

void print_memory_usage(const char *name) {
    struct mallinfo2 mi = mallinfo2();

    printf("\n\n----------------------------------------------------------------\n");
    printf("Memory usage at %s\n", name);
    printf("Total allocated space: %s\n", human_memory_size(mi.uordblks));
    printf("Total free space: %s\n", human_memory_size(mi.fordblks));
    printf("Total releasable space: %s\n", human_memory_size(mi.keepcost));
    printf("-----------------------------------------------------------------\n\n");
}