#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "objects.h"

void * rotate(Object);
void * translate(Object, Point_d *);
void * reflect(Node_ptr, bool);
void *scale(Node_ptr, bool, bool, bool);

#endif