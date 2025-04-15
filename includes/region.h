#ifndef REGION_H
#define REGION_H
#include <stdbool.h>

typedef struct {
    int id;
    char name[100];
    char capital[100];
    int population;
    double square;
    int country_id;
} Region;

bool add_region(Region *region);
bool delete_region(int region_id);
void print_regions_by_country(int country_id);
#endif
