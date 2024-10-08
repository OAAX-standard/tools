#include "logger.h"

Logger* create_logger(const char *filename, LogLevel file_level, LogLevel console_level) {
    Logger *logger = (Logger*) malloc(sizeof(Logger));
    if (logger == NULL) {
        perror("Failed to allocate memory for logger");
        exit(EXIT_FAILURE);
    }

    logger->filename = strdup(filename);  // Save the base filename
    logger->file_index = 0;
    logger->file_level = file_level;
    logger->console_level = console_level;
    pthread_mutex_init(&logger->lock, NULL);

    // Open the initial log file
    char log_filename[1024];
    snprintf(log_filename, sizeof(log_filename), "%s.%d", logger->filename, logger->file_index);
    logger->log_file = fopen(log_filename, "a");
    if (logger->log_file == NULL) {
        perror("Failed to open log file");
        free(logger->filename);
        free(logger);
        exit(EXIT_FAILURE);
    }

    return logger;
}

void close_logger(Logger *logger) {
    if (logger->log_file != NULL) {
        fclose(logger->log_file);
    }
    pthread_mutex_destroy(&logger->lock);
    free(logger->filename);
    free(logger);
}

void rotate_log_file(Logger *logger) {
    fclose(logger->log_file);
    logger->file_index++;

    char log_filename[256];
    snprintf(log_filename, sizeof(log_filename), "%s_%d.log", logger->filename, logger->file_index);
    logger->log_file = fopen(log_filename, "a");
    if (logger->log_file == NULL) {
        perror("Failed to open new log file");
        exit(EXIT_FAILURE);
    }
}

void log_message(Logger *logger, LogLevel level, const char *format, ...) {
    pthread_mutex_lock(&logger->lock);

    // Check the size of the log file
    fseek(logger->log_file, 0, SEEK_END);
    long fileSize = ftell(logger->log_file);
    while (fileSize >= MAX_LOG_FILE_SIZE) {
        rotate_log_file(logger);
    }
    fseek(logger->log_file, 0, SEEK_END);  // Reset file pointer to end

    const char *level_str;
    switch (level) {
        case LOG_DEBUG:
            level_str = "DEBUG";
            break;
        case LOG_INFO:
            level_str = "INFO";
            break;
        case LOG_WARNING:
            level_str = "WARNING";
            break;
        case LOG_ERROR:
            level_str = "ERROR";
            break;
        default:
            level_str = "UNKNOWN";
            break;
    }

    time_t now = time(NULL);
    struct tm *localTime = localtime(&now);
    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localTime);

    va_list args;
    va_start(args, format);

    // build the message as "[time] LEVEL: message"
    char message[1024];
    vsnprintf(message, sizeof(message), format, args);
    if (level >= logger->file_level && logger->log_file != NULL) {
        fprintf(logger->log_file, "[%s] %s: %s\n", timeStr, level_str, message);
    }

    // Print to console if the log level is high enough
    if (level >= logger->console_level) {
        printf("[%s] %s: %s\n", timeStr, level_str, message);
    }

    va_end(args);
    pthread_mutex_unlock(&logger->lock);
}
