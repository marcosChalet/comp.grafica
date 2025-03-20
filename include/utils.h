#ifndef UTILS_h
#define UTILS_h

#define to_string(obj) ((obj)->to_string(obj))
#define draw(obj) ((obj)->draw(obj))

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

void verify_allocation_error(const void *);

#endif
