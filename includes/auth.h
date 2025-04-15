#ifndef AUTH_H
#define AUTH_H
#include <stdbool.h>
bool login(const char *username, const char *password);
void logout();
#endif
