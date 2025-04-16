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
    input_str("Country name (required): ", c.name, sizeof(c.name));
    if (strlen(c.name) == 0) {
        printf("Error: Country name cannot be empty!\n");
        return;
    }
    input_str("Capital (optional): ", c.capital, sizeof(c.capital));
    input_str("Official language (optional): ", c.language, sizeof(c.language));
    printf("Population (default 0): ");
    scanf("%d", &c.population_country);
    printf("Area (km², default 0.0): ");
    scanf("%lf", &c.square_country);  // 使用 square
    getchar();
    input_str("Currency code (e.g., USD, optional): ", c.currency, sizeof(c.currency));
    input_str("Head of state (optional): ", c.head_country, sizeof(c.head_country));  // 使用 head

    if (add_country(&c)) printf("Country added successfully!\n");
    else printf("Failed to add country!\n");
}

void add_region_ui() {
    Region r = {0};
    printf("Country ID: ");
    scanf("%d", &r.country_id);
    getchar();
    input_str("Region name: ", r.name, sizeof(r.name));
    input_str("Capital: ", r.capital, sizeof(r.capital));  // 使用 capital
    printf("Population: ");
    scanf("%d", &r.population);
    printf("Area (km²): ");
    scanf("%lf", &r.square);  // 使用 square
    if (add_region(&r)) printf("Region added successfully!\n");
    else printf("Failed to add region!\n");
}

void main_menu() {
    int choice;
    do {
        printf("\n=== Country Management System ===\n");
        printf("1. Add Country\n2. Delete Country\n3. List Countries\n4. Add Region\n");
        printf("5. Delete Region\n6. List Regions by Country\n7. Calculate Avg Region Population\n");
        printf("8. Calculate Total Population\n0. Exit\nChoose an option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: add_country_ui(); break;
            case 2: {
                int id;
                printf("Enter country ID to delete: ");
                scanf("%d", &id);
                delete_country(id) ? printf("Country deleted!\n") : printf("Deletion failed!\n");
                break;
            }
            case 3: print_countries(); break;
            case 4: add_region_ui(); break;
            case 5: {
                int id;
                printf("Enter region ID to delete: ");
                scanf("%d", &id);
                delete_region(id) ? printf("Region deleted!\n") : printf("Deletion failed!\n");
                break;
            }
            case 6: {  // 修复：添加代码块并声明 id
                int id;
                printf("Enter country ID: ");
                scanf("%d", &id);
                print_regions_by_country(id);
                break;
            }
            case 7: {
                int id;
                printf("Enter country ID: ");
                scanf("%d", &id);
                float avg = get_avg_region_population(id);
                printf("Average region population: %.2f\n", avg);
                break;
            }
            case 8: {
                int total = get_total_population();
                printf("Total population: %d\n", total);
                break;
            }
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid option!\n");
        }
    } while (choice != 0);
}
int main() {
    if (!initialize_database("src/countries.db")) {
        fprintf(stderr, "Database initialization failed!\n");
        return 1;
    }

    if (!table_exists("country")) {
execute_sql(
    "CREATE TABLE country ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "name TEXT NOT NULL, "
    "capital TEXT, "
    "language TEXT, "
    "population_country INTEGER DEFAULT 0, "  // 确保列名与插入语句匹配
    "square_country REAL DEFAULT 0.0, "
    "currency TEXT, "
    "head_country TEXT"
    ");"
);        execute_sql(
            "CREATE TABLE region ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT NOT NULL, "
            "capital TEXT, "  // 字段名统一为 capital
            "population INTEGER DEFAULT 0, "
            "square REAL DEFAULT 0.0, "  // 字段名统一为 square
            "country_id INTEGER, "
            "FOREIGN KEY (country_id) REFERENCES country(id) ON DELETE CASCADE"
            ");"
        );
    }

    main_menu();
    close_database();
    return 0;
}
