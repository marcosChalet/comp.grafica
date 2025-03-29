#include <stdio.h>
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