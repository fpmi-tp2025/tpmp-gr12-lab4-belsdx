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
    input_str("Country Name (required): ", c.name, sizeof(c.name));
    if (strlen(c.name) == 0) {
        printf("Error: Country name cannot be empty！\n");
        return;
    }
    input_str("Capital (optional): ", c.capital, sizeof(c.capital));
    input_str("Official language (optional): ", c.language, sizeof(c.language));
    printf("Population (million, default 0): ");
    scanf("%d", &c.population);
    printf("Area (square kilometers, default 0.0): ");
    scanf("%lf", &c.square);
    getchar();
    input_str("Currency (such as CNY, optional): ", c.currency, sizeof(c.currency));
    input_str("Head of State (optional): ", c.head, sizeof(c.head));

    if (add_country(&c)) printf("Country added successfully！\n");
    else printf("Add failed！\n");
}

void add_region_ui() {
    Region r = {0};
    printf("Country ID: ");
    scanf("%d", &r.country_id);
    getchar();
    input_str("Region Name: ", r.name, sizeof(r.name));
    input_str("Capital: ", r.capital, sizeof(r.capital));
    printf("population: ");
    scanf("%d", &r.population);
    printf("area（Square Kilometer）: ");
    scanf("%lf", &r.square);
    if (add_region(&r)) printf("Area added successfully！\n");
    else printf("Add failed！\n");
}

void main_menu() {
    int choice;
    do {
        printf("\n=== National Management System ===\n");
        printf("1. Add Country\n2. Delete Country\n3. Show Country\n4. Adding a Region\n5. Delete a region\n");
        printf("6. Display regions of a country\n7. Calculate the average population of a region\n8. Calculate total population\n0. quit\nPlease select an operation: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: add_country_ui(); break;
            case 2: {
                int id;
                printf("Enter the country ID to be deleted: ");
                scanf("%d", &id);
                delete_country(id) ? printf("Deleted successfully！\n") : printf("Deletion failed！\n");
                break;
            }
            case 3: print_countries(); break;
            case 4: add_region_ui(); break;
            case 5: {
                int id;
                printf("Enter the region ID to be deleted: ");
                scanf("%d", &id);
                delete_region(id) ? printf("Deleted successfully！\n") : printf("Deletion failed！\n");
                break;
            }
            case 6: {
                int id;
                printf("Input Country ID: ");
                scanf("%d", &id);
                print_regions_by_country(id);
                break;
            }
            case 7: {
                int id;
                printf("Input Country ID: ");
                scanf("%d", &id);
                float avg = get_avg_region_population(id);
                printf("Average population: %.2f\n", avg);
                break;
            }
            case 8: {
                int total = get_total_population();
                printf("Total population: %d\n", total);
                break;
            }
            case 0: printf("See you！\n"); break;
            default: printf("Invalid option！\n");
        }
    } while (choice != 0);
}

int main() {
    if (!initialize_database("src/countries.db")) {
        fprintf(stderr, "Database initialization failed！\n");
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
