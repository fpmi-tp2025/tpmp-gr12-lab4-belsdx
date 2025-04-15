#ifndef DATABASE_H
#define DATABASE_H
#include <sqlite3.h>
#include <stdbool.h>

extern sqlite3 *db;

bool initialize_database(const char *db_name);
void close_database();
bool execute_sql(const char *sql);
bool table_exists(const char *table_name);
#endif
