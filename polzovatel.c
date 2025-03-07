#include "polzovatel.h"
#include "logs.h"
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>

#define MIN_USER_ID 1000

void list_polzovatel() {
    struct passwd *user_info;
    FILE *output_stream = log_file ? log_file : stdout;

    while ((user_info = getpwent()) != NULL) {
        if (user_info->pw_uid >= MIN_USER_ID) {
            fprintf(output_stream, "%s\n", user_info->pw_name);
        }
    }

    endpwent();
}

