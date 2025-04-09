#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

char * enum_to_string(int mode) {
    char * str_mode;
    
    switch (mode)
    {
        case CREATING_POINT:
            str_mode = malloc(sizeof(char)*15);
            strcpy(str_mode, "CREATING_POINT");
            str_mode[14] = '\0';
            break;
        case CREATING_LINE:
            str_mode = malloc(sizeof(char)*14);
            strcpy(str_mode, "CREATING_LINE");
            str_mode[13] = '\0';
            break;
        case CREATING_POLYGON:
            str_mode = malloc(sizeof(char)*17);
            strcpy(str_mode, "CREATING_POLYGON");
            str_mode[16] = '\0';
            break;
        case VIEW_MODE:
            str_mode = malloc(sizeof(char)*10);
            strcpy(str_mode, "VIEW_MODE");
            str_mode[9] = '\0';
            break;
        case DELETE_OBJECT:
            str_mode = malloc(sizeof(char)*14);
            strcpy(str_mode, "DELETE_OBJECT");
            str_mode[13] = '\0';
            break;
        case ROTATE:
            str_mode = malloc(sizeof(char)*7);
            strcpy(str_mode, "ROTATE");
            str_mode[6] = '\0';
            break;
        case SELECT:
            str_mode = malloc(sizeof(char)*7);
            strcpy(str_mode, "SELECT");
            str_mode[6] = '\0';
            break;
        case TRANSLATE:
            str_mode = malloc(sizeof(char)*10);
            strcpy(str_mode, "TRANSLATE");
            str_mode[9] = '\0';
            break;
        case SCALE_UP:
            str_mode = malloc(sizeof(char)*9);
            strcpy(str_mode, "SCALE_UP");
            str_mode[8] = '\0';
            break;
        case SCALE_UP_X:
            str_mode = malloc(sizeof(char)*9);
            strcpy(str_mode, "SCALE_UP");
            str_mode[8] = '\0';
            break;
        case SCALE_UP_Y:
            str_mode = malloc(sizeof(char)*9);
            strcpy(str_mode, "SCALE_UP");
            str_mode[8] = '\0';
            break;
        case SCALE_DOWN:
            str_mode = malloc(sizeof(char)*11);
            strcpy(str_mode, "SCALE_DOWN");
            str_mode[10] = '\0';
            break;
        case SCALE_DOWN_X:
            str_mode = malloc(sizeof(char)*11);
            strcpy(str_mode, "SCALE_DOWN");
            str_mode[10] = '\0';
            break;
        case SCALE_DOWN_Y:
            str_mode = malloc(sizeof(char)*11);
            strcpy(str_mode, "SCALE_DOWN");
            str_mode[10] = '\0';
            break;
        case REFLECT:
            str_mode = malloc(sizeof(char)*8);
            strcpy(str_mode, "REFLECT");
            str_mode[7] = '\0';
            break;
        case SHEAR:
            str_mode = malloc(sizeof(char)*6);
            strcpy(str_mode, "SHEAR");
            str_mode[5] = '\0';
            break;
        case TO_CONVEX:
            str_mode = malloc(sizeof(char)*10);
            strcpy(str_mode, "TO_CONVEX");
            str_mode[9] = '\0';
            break;
        default:
            str_mode = NULL;
            break;
    }

    return str_mode;
}