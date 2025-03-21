#ifndef UTILS_h
#define UTILS_h

#include <stdbool.h>

#define FILENAME "./data/objects.db"
#define BUFFER_SIZE 10000
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

#define to_string(obj) ((obj)->to_string(obj))
#define draw(obj) ((obj)->draw(obj))

void verify_allocation_error(const void *);
bool file_error(FILE *);
int mode_to_type(int);

#endif
