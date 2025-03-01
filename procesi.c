#include "processes.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#define PROC_DIRECTORY "/proc"
#define CMDLINE_BUFFER_SIZE 512

void display_processes() {
    DIR *proc_dir = opendir(PROC_DIRECTORY);
    if (!proc_dir) {
        fprintf(stderr, "Error: Unable to open directory %s\n", PROC_DIRECTORY);
        return;
    }

    struct dirent *dir_entry;
    FILE *output_stream = log_file ? log_file : stdout;

    while ((dir_entry = readdir(proc_dir)) != NULL) {
        if (!isdigit(dir_entry->d_name[0])) continue;

        char command_line[CMDLINE_BUFFER_SIZE] = {0};

        char command_path[CMDLINE_BUFFER_SIZE];
        snprintf(command_path, sizeof(command_path), "%s/%s/cmdline", PROC_DIRECTORY, dir_entry->d_name);
        FILE *command_file = fopen(command_path, "r");

        if (command_file) {
            if (fgets(command_line, CMDLINE_BUFFER_SIZE, command_file) != NULL) {
                fclose(command_file);
            } else {
                fclose(command_file);
                command_line[0] = '\0';
            }
        }

        if (command_line[0] == '\0') {
            char executable_path[CMDLINE_BUFFER_SIZE];
            snprintf(executable_path, sizeof(executable_path), "%s/%s/exe", PROC_DIRECTORY, dir_entry->d_name);
            ssize_t length = readlink(executable_path, command_line, CMDLINE_BUFFER_SIZE - 1);
            if (length != -1) {
                command_line[length] = '\0';
            } else {
                command_line[0] = '\0';
            }
        }

        if (command_line[0] == '\0') {
            char status_path[CMDLINE_BUFFER_SIZE];
            snprintf(status_path, sizeof(status_path), "%s/%s/status", PROC_DIRECTORY, dir_entry->d_name);
            FILE *status_file = fopen(status_path, "r");

            if (status_file) {
                char status_line[CMDLINE_BUFFER_SIZE];
                while (fgets(status_line, sizeof(status_line), status_file)) {
                    if (strncmp(status_line, "Name:", 5) == 0) {
                        sscanf(status_line, "Name:\t%511s", command_line);
                        break;
                    }
                }
                fclose(status_file);
            }
        }

        if (command_line[0] == '\0') continue;

        fprintf(output_stream, "PID: %s, Process: %s\n", dir_entry->d_name, command_line);
    }

    closedir(proc_dir);
}