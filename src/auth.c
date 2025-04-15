#include "auth.h"
#include <string.h>

bool login(const char *username, const char *password) {
    return (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0);
}

void logout() {}
