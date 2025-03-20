#include <stdio.h>
#include <errno.h>
#include "utils.h"

void verify_allocation_error(const void * ptr) {
    if (ptr == NULL) perror(RED "Possible memory allocation error" RESET);
}