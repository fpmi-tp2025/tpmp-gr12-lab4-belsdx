#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auth.h"
#include "database.h"
#include "country.h"
#include "region.h"

void input_str(const char *prompt, char *buffer, int max_len) {
    printf("%s", prompt);
    fgets(buffer, max_len, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

void add_country_ui() {
    Country c = {0};
    input_str("国家名称（必填）: ", c.name, sizeof(c.name));
    if (strlen(c.name) == 0) {
        printf("错误：国家名称不能为空！\n");
        return;
    }
    input_str("首都（可选）: ", c.capital, sizeof(c.capital));
    input_str("官方语言（可选）: ", c.language, sizeof(c.language));
    printf("人口（万，默认0）: ");
    scanf("%d", &c.population);
    printf("面积（平方公里，默认0.0）: ");
    scanf("%lf", &c.square);
    getchar();
    input_str("货币代码（如CNY，可选）: ", c.currency, sizeof(c.currency));
    input_str("国家元首（可选）: ", c.head, sizeof(c.head));

    if (add_country(&c)) printf("国家添加成功！\n");
    else printf("添加失败！\n");
}

void add_region_ui() {
    Region r = {0};
    printf("所属国家ID: ");
    scanf("%d", &r.country_id);
    getchar();
    input_str("区域名称: ", r.name, sizeof(r.name));
    input_str("首府: ", r.capital, sizeof(r.capital));
    printf("人口: ");
    scanf("%d", &r.population);
    printf("面积（平方公里）: ");
    scanf("%lf", &r.square);
    if (add_region(&r)) printf("区域添加成功！\n");
    else printf("添加失败！\n");
}

void main_menu() {
    int choice;
    do {
        printf("\n=== 国家管理系统 ===\n");
        printf("1. 添加国家\n2. 删除国家\n3. 显示国家\n4. 添加区域\n5. 删除区域\n");
        printf("6. 显示某国家的区域\n7. 计算区域平均人口\n8. 计算总人口\n0. 退出\n请选择操作: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: add_country_ui(); break;
            case 2: {
                int id;
                printf("输入要删除的国家ID: ");
                scanf("%d", &id);
                delete_country(id) ? printf("删除成功！\n") : printf("删除失败！\n");
                break;
            }
            case 3: print_countries(); break;
            case 4: add_region_ui(); break;
            case 5: {
                int id;
                printf("输入要删除的区域ID: ");
                scanf("%d", &id);
                delete_region(id) ? printf("删除成功！\n") : printf("删除失败！\n");
                break;
            }
            case 6: {
                int id;
                printf("输入国家ID: ");
                scanf("%d", &id);
                print_regions_by_country(id);
                break;
            }
            case 7: {
                int id;
                printf("输入国家ID: ");
                scanf("%d", &id);
                float avg = get_avg_region_population(id);
                printf("平均人口: %.2f\n", avg);
                break;
            }
            case 8: {
                int total = get_total_population();
                printf("总人口: %d\n", total);
                break;
            }
            case 0: printf("再见！\n"); break;
            default: printf("无效选项！\n");
        }
    } while (choice != 0);
}

int main() {
    if (!initialize_database("src/countries.db")) {
        fprintf(stderr, "数据库初始化失败！\n");
        return 1;
    }

    if (!table_exists("country")) {
        execute_sql(
            "CREATE TABLE country ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT NOT NULL, "
            "capital TEXT, "
            "language TEXT, "
            "population_country INTEGER DEFAULT 0, "
            "square_country REAL DEFAULT 0.0, "
            "currency TEXT, "
            "head_country TEXT"
            ");"
        );
        execute_sql(
            "CREATE TABLE region ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT NOT NULL, "
            "capital_region TEXT, "
            "population_region INTEGER DEFAULT 0, "
            "square_region REAL DEFAULT 0.0, "
            "country_id INTEGER, "
            "FOREIGN KEY (country_id) REFERENCES country(id) ON DELETE CASCADE"
            ");"
        );
    }

    main_menu();
    close_database();
    return 0;
}
