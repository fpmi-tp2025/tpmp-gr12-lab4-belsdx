#include "database.h"
#include <stdio.h>
#include <string.h>

sqlite3 *db = NULL;

bool initialize_database(const char *db_name) {
    printf("正在连接数据库: %s\n", db_name);
    if (sqlite3_open(db_name, &db) != SQLITE_OK) {
        fprintf(stderr, "无法打开数据库: %s\n", sqlite3_errmsg(db));
        return false;
    }
    if (sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL) != SQLITE_OK) {
        fprintf(stderr, "外键启用失败: %s\n", sqlite3_errmsg(db));
        return false;
    }
    return true;
}

void close_database() {
    if (db) sqlite3_close(db);
}

bool execute_sql(const char *sql) {
    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "SQL错误: %s\n", err_msg);
        sqlite3_free(err_msg);
        return false;
    }
    return true;
}

bool table_exists(const char *table_name) {
    char sql[256];
    snprintf(sql, sizeof(sql), 
        "SELECT count(*) FROM sqlite_master WHERE type='table' AND name='%s';", 
        table_name
    );
    sqlite3_stmt *stmt;
    bool exists = false;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = (sqlite3_column_int(stmt, 0) == 1);
        }
        sqlite3_finalize(stmt);
    }
    return exists;
}
