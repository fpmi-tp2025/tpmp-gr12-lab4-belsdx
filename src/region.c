#include "region.h"
#include "database.h"
#include <stdio.h>
#include <string.h>

bool add_region(Region *region) {
    char sql[512];
    snprintf(sql, sizeof(sql),
        "INSERT INTO region (name, capital, population, square, country_id) "
        "VALUES ('%s', '%s', %d, %.2f, %d);",
        region->name, region->capital, region->population, region->square, region->country_id
    );
    return execute_sql(sql);
}

bool delete_region(int region_id) {
    char sql[128];
    snprintf(sql, sizeof(sql), "DELETE FROM region WHERE id = %d;", region_id);
    return execute_sql(sql);
}

void print_regions_by_country(int country_id) {
    char sql[256];
    snprintf(sql, sizeof(sql), 
        "SELECT id, name, capital, population FROM region WHERE country_id = %d;", 
        country_id
    );
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        printf("ID | Region Name | Capital | Population\n");
        printf("---------------------------------------\n");
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
