#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "utils.h"
#include "events.h"

void verify_allocation_error(const void * ptr) {
    if (ptr == NULL) perror(RED "Possible memory allocation error" RESET);
}

int mode_to_type(int mode) {
    switch (mode) {
        case CREATING_POINT   : return POINT_T;
        case CREATING_LINE    : return LINE_T;
        case CREATING_POLYGON : return POLYGON_T;
    }
}

bool file_error(FILE* fp) {
    if(fp != NULL) return false;
    perror(RED "File error" RESET);
    return true;
}

RegionCode compute_out_code(int x, int y, int x_max, int x_min, int y_max, int y_min) {
    RegionCode code = INSIDE;
    
    if (x < x_min) code |= LEFT;
    else if (x > x_max) code |= RIGHT;
    if (y < y_min) code |= BOTTOM;
    else if (y > y_max) code |= TOP;

    return code;
}