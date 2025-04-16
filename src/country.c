#include "country.h"
#include "database.h"
#include <stdio.h>
#include <string.h>

bool add_country(Country *country) {
    const char *sql = 
        "INSERT INTO country (name, capital, language, population_country, square_country, currency, head_country) "
        "VALUES (?, ?, ?, ?, ?, ?, ?);";    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL Error: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_text(stmt, 1, country->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, country->capital, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, country->language, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, country->population_country);
    sqlite3_bind_double(stmt, 5, country->square_country);
    sqlite3_bind_text(stmt, 6, country->currency, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, country->head_country, -1, SQLITE_STATIC);
    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool delete_country(int country_id) {
    char sql[128];
    snprintf(sql, sizeof(sql), "DELETE FROM country WHERE id = %d;", country_id);
    return execute_sql(sql);
}

void print_countries() {
    const char *sql = "SELECT id, name, capital, population_country FROM country;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        printf("ID | Country Name | Capital | Population_country\n");
        printf("----------------------------------------\n");
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("%-2d | %-12s | %-10s | %d\n",
                sqlite3_column_int(stmt, 0),
                (const char*)sqlite3_column_text(stmt, 1),
                (const char*)sqlite3_column_text(stmt, 2),
                sqlite3_column_int(stmt, 3)
            );
        }
        sqlite3_finalize(stmt);
    }
}

float get_avg_region_population(int country_id) {
    char sql[256];
    snprintf(sql, sizeof(sql),
        "SELECT AVG(population) FROM region WHERE country_id = %d;",
        country_id
    );
    sqlite3_stmt *stmt;
    float avg = 0.0;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            avg = (float)sqlite3_column_double(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    return avg;
}

int get_total_population() {
    const char *sql = "SELECT SUM(population_country) FROM country;";
    sqlite3_stmt *stmt;
    int total = 0;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            total = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    return total;
}
