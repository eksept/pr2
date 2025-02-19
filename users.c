#include "users.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>

#define MIN_UID 1000

void list_users() {
    struct passwd *pwd;
    FILE *output = log_file ? log_file : stdout;

    while ((pwd = getpwent()) != NULL) {
        if (pwd->pw_uid >= MIN_UID) { 
            fprintf(output, "%s\n", pwd->pw_name);
        }
    }

    endpwent();
}

