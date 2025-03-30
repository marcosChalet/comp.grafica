#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "utils.h"
#include "transformations.h"
#include "opengl_functions.h"

#define ANGLE 30
#define M_PI acos(-1.0)

void rotate_point(Point_d *p, Point_d *origin)
{
    double radians = ANGLE * (M_PI / 180.0);
    double x_shifted = p->x - origin->x;
    double y_shifted = p->y - origin->y;

    double x_new = x_shifted * cos(radians) - y_shifted * sin(radians);
    double y_new = x_shifted * sin(radians) + y_shifted * cos(radians);

    p->x = x_new + origin->x;
    p->y = y_new + origin->y;
}

void rotate_line(Line_d *l)
{
    Point_d mid;
    mid.x = (l->s_point->x + l->e_point->x) / 2.0;
    mid.y = (l->s_point->y + l->e_point->y) / 2.0;
    rotate_point(l->e_point, &mid);
    rotate_point(l->s_point, &mid);
}

void rotate_polygon(Polygon_d *p)
{
    Point_d centroid = {0, 0};

    Node_ptr *all_points = get_all(p->vertices);

    for (int i = 0; i < p->vertices->num_objects; i++)
    {
        centroid.x += ((Point_d *)all_points[i]->object)->x;
        centroid.y += ((Point_d *)all_points[i]->object)->y;
    }

    centroid.x /= p->vertices->num_objects;
    centroid.y /= p->vertices->num_objects;

    for (int i = 0; i < p->vertices->num_objects; i++)
    {
        rotate_point((Point_d *)all_points[i]->object, &centroid);
    }
}

void *rotate(Object o)
{

    if (o == NULL || ((Node_ptr)o)->object == NULL)
    {
        perror(RED "ERROR: O objeto não foi selecionado\n" RESET);
        return NULL;
    }

    glut_post_redisplay();
    switch (((Node_ptr)o)->type)
    {
    case LINE_T:
        rotate_line(((Node_ptr)o)->object);
        break;
    case POLYGON_T:
        rotate_polygon(((Node_ptr)o)->object);
        break;
    default:
        break;
    }

    return NULL;
}

void change_point(Point_d *point, const float matrix[3][3])
{
    point->x = round(point->x * matrix[0][0] + point->y * matrix[0][1] + matrix[0][2]);
    point->y = round(point->x * matrix[1][0] + point->y * matrix[1][1] + matrix[1][2]);
}

void translate_point(Point_d *point, Point_d *final_position)
{
    double dx = final_position->x - point->x;
    double dy = final_position->y - point->y;

    float matrix[3][3] = {
        {1.0, 0.0, dx},
        {0.0, 1.0, dy},
        {0.0, 0.0, 1.0}};

    change_point(point, matrix);
}

void translate_line(Line_d *line, Point_d *final_position)
{
    if (line == NULL || final_position == NULL ||
        line->s_point == NULL || line->e_point == NULL)
        return;

    double dx = final_position->x - line->s_point->x;
    double dy = final_position->y - line->s_point->y;

    float translationMatrix[3][3] = {
        {1.0, 0.0, dx},
        {0.0, 1.0, dy},
        {0.0, 0.0, 1.0}};

    change_point(line->s_point, translationMatrix);
    change_point(line->e_point, translationMatrix);
}

void translate_polygon(Polygon_d *polygon, Point_d *final_centroid)
{
    if (polygon == NULL || final_centroid == NULL || polygon->vertices->num_objects <= 0)
        return;

    double cx = 0.0, cy = 0.0;
    Node_ptr *all_vertices = get_all(polygon->vertices);

    for (int i = 0; i < polygon->vertices->num_objects; i++)
    {
        cx += ((Point_d *)all_vertices[i]->object)->x;
        cy += ((Point_d *)all_vertices[i]->object)->y;
    }
    cx /= polygon->vertices->num_objects;
    cy /= polygon->vertices->num_objects;

    double dx = final_centroid->x - cx;
    double dy = final_centroid->y - cy;

    float translationMatrix[3][3] = {
        {1.0, 0.0, dx},
        {0.0, 1.0, dy},
        {0.0, 0.0, 1.0}};

    for (int i = 0; i < polygon->vertices->num_objects; i++)
    {
        change_point((Point_d *)all_vertices[i]->object, translationMatrix);
    }
}

void *translate(Object o, Point_d *p)
{

    if (o == NULL || ((Node_ptr)o)->object == NULL)
    {
        perror(RED "ERROR: O objeto não foi selecionado\n" RESET);
        return NULL;
    }

    switch (((Node_ptr)o)->type)
    {
    case POINT_T:
        translate_point(((Node_ptr)o)->object, p);
        break;
    case LINE_T:
        translate_line(((Node_ptr)o)->object, p);
        break;
    case POLYGON_T:
        translate_polygon(((Node_ptr)o)->object, p);
        break;
    default:
        break;
    }

    return NULL;
}

// TODO: Functions to reflect feature
void reflect_point(Point_d *p, Point_d *origin, bool isVerticalFlip)
{
    if (p == NULL)
    {
        perror(RED "ERROR: O ponto é invalido\n" RESET);
        return;
    }

    double x_shifted = p->x - origin->x;
    double y_shifted = p->y - origin->y;

    if (isVerticalFlip)
    {
        p->y = origin->y - y_shifted;
    }
    else
    {
        p->x = origin->x - x_shifted;
    }
}

void reflect_line(Line_d *l, bool isVerticalFlip)
{
    Point_d mid;
    mid.x = (l->s_point->x + l->e_point->x) / 2.0;
    mid.y = (l->s_point->y + l->e_point->y) / 2.0;

    reflect_point(l->e_point, &mid, isVerticalFlip);

    reflect_point(l->s_point, &mid, isVerticalFlip);
}

void reflect_polygon(Polygon_d *p, bool isVerticalFlip)
{

    Point_d centroid = {0, 0};

    Node_ptr *all_points = get_all(p->vertices);

    for (int i = 0; i < p->vertices->num_objects; i++)
    {
        centroid.x += ((Point_d *)all_points[i]->object)->x;
        centroid.y += ((Point_d *)all_points[i]->object)->y;
    }

    centroid.x /= p->vertices->num_objects;
    centroid.y /= p->vertices->num_objects;

    for (int i = 0; i < p->vertices->num_objects; i++)
    {
        reflect_point((Point_d *)all_points[i]->object, &centroid, isVerticalFlip);
    }
}

void *reflect(Node_ptr o, bool isVerticalFlip)
{
    if (o == NULL || ((Node_ptr)o)->object == NULL)
    {
        perror(RED "ERROR: O objeto não foi selecionado\n" RESET);
        return NULL;
    }

    glut_post_redisplay();

    switch (((Node_ptr)o)->type)
    {
    case LINE_T:
        reflect_line(((Node_ptr)o)->object, isVerticalFlip);
        break;
    case POLYGON_T:
        reflect_polygon(((Node_ptr)o)->object, isVerticalFlip);
        break;
    default:
        break;
    }

    return NULL;
}