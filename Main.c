#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "polzovatel.h"
#include "procesi.h"
#include "logs.h"

void print_help() {
    printf("Использование: myutil [Настройки]\n");
    printf("  -u, --users        Показывает список пользователей\n");
    printf("  -p, --processes    Показывает процессы\n");
    printf("  -h, --help         Вывод справки\n");
    printf("  -l, --log PATH     Запись в лог PATH\n");
    printf("  -e, --errors PATH  Ошибки при записи в лог PATH\n");
}

int open_log_file(const char *path) {
    FILE *file = fopen(path, "w");
    if (!file) {
        fprintf(stderr, "Ошибка: Недоступна запись в лог: %s\n", path);
        return 0;
    }
    log_file = file;
    return 1;
}

int open_error_file(const char *path) {
    FILE *file = fopen(path, "w");
    if (!file) {
        fprintf(stderr, "Ошибка: Невозможно записать ошибки в лог: %s\n", path);
        return 0;
    }
    error_file = file;
    return 1;
}

int main(int argc, char *argv[]) {
    int option;
    int polzovatel_flag = 0, procesi_flag = 0;

    struct option long_options[] = {
        {"polzovatel", no_argument, NULL, 'u'},
        {"procesi", no_argument, NULL, 'p'},
        {"help", no_argument, NULL, 'h'},
        {"log", required_argument, NULL, 'l'},
        {"errors", required_argument, NULL, 'e'},
        {NULL, 0, NULL, 0}
    };

    while ((option = getopt_long(argc, argv, "uph:l:e:", long_options, NULL)) != -1) {
        switch (option) {
            case 'u':
                polzovatel_flag = 1;
                break;
            case 'p':
                procesi_flag = 1;
                break;
            case 'h':
                print_help();
                return 0;
            case 'l':
                if (!open_log_file(optarg)) return 1;
                break;
            case 'e':
                if (!open_error_file(optarg)) return 1;
                break;
            default:
                print_help();
                return 1;
        }
    }

    if (polzovatel_flag) list_polzovatel();
    if (procesi_flag) list_procesi();

    if (log_file) fclose(log_file);
    if (error_file) fclose(error_file);

    return 0;
}
