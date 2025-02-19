#include "processes.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#define PROC_DIR "/proc"
#define CMDLINE_MAX 512

void list_processes() {
    DIR *dir = opendir(PROC_DIR);
    if (!dir) {
        fprintf(stderr, "Error: Cannot open %s directory\n", PROC_DIR);
        return;
    }

    struct dirent *entry;
    FILE *output = log_file ? log_file : stdout;

    while ((entry = readdir(dir)) != NULL) {
        if (!isdigit(entry->d_name[0])) continue; 

        char cmdline[CMDLINE_MAX] = {0};

        char cmd_path[CMDLINE_MAX];
        snprintf(cmd_path, sizeof(cmd_path), "%s/%s/cmdline", PROC_DIR, entry->d_name);
        FILE *cmd_file = fopen(cmd_path, "r");

        if (cmd_file) {
            if (fgets(cmdline, CMDLINE_MAX, cmd_file) != NULL) {
                fclose(cmd_file);
            } else {
                fclose(cmd_file);
                cmdline[0] = '\0'; 
            }
        }

        if (cmdline[0] == '\0') {
            char exe_path[CMDLINE_MAX];
            snprintf(exe_path, sizeof(exe_path), "%s/%s/exe", PROC_DIR, entry->d_name);
            ssize_t len = readlink(exe_path, cmdline, CMDLINE_MAX - 1);
            if (len != -1) {
                cmdline[len] = '\0'; 
            } else {
                cmdline[0] = '\0'; 
            }
        }

        if (cmdline[0] == '\0') {
            char status_path[CMDLINE_MAX];
            snprintf(status_path, sizeof(status_path), "%s/%s/status", PROC_DIR, entry->d_name);
            FILE *status_file = fopen(status_path, "r");

            if (status_file) {
                char line[CMDLINE_MAX];
                while (fgets(line, sizeof(line), status_file)) {
                    if (strncmp(line, "Name:", 5) == 0) {
                        sscanf(line, "Name:\t%511s", cmdline);
                        break;
                    }
                }
                fclose(status_file);
            }
        }

        if (cmdline[0] == '\0') continue;

        fprintf(output, "PID: %s, Process: %s\n", entry->d_name, cmdline);
    }

    closedir(dir);
}

