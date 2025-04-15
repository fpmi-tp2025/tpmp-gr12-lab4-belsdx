#ifndef COUNTRY_H
#define COUNTRY_H
#include <stdbool.h>

typedef struct {
    int id;
    char name[100];
    char capital[100];
    char language[50];
    int population;
    double square;
    char currency[4];
    char head[100];
} Country;

bool add_country(Country *country);
bool delete_country(int country_id);
void print_countries();
float get_avg_region_population(int country_id);
int get_total_population();
#endif
