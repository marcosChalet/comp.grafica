#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void veirfy_allocation_error(const void * ptr) {
    if (ptr == NULL) perror("Possible memory allocation error!");
}