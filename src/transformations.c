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
    printf("transladando node\n");
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

// TODO: Functions to scale feature

void scale_point(Point_d *p, Point_d *origin, double scale_factor_x, double scale_factor_y)
{

    if (p == NULL || origin == NULL)
    {
        perror("ERROR: O ponto ou a origem é inválido\n");
        return;
    }

    double x_shifted = p->x - origin->x;
    double y_shifted = p->y - origin->y;

    x_shifted = scale_factor_x * x_shifted;
    y_shifted = scale_factor_y * y_shifted;

    p->x = x_shifted + origin->x;
    p->y = y_shifted + origin->y;
}

void scale_line(Line_d *l, double scale_factor_x, double scale_factor_y)
{

    Point_d mid;
    mid.x = (l->s_point->x + l->e_point->x) / 2.0;
    mid.y = (l->s_point->y + l->e_point->y) / 2.0;

    scale_point(l->s_point, &mid, scale_factor_x, scale_factor_y);
    scale_point(l->e_point, &mid, scale_factor_x, scale_factor_y);
}

void scale_polygon(Polygon_d *p, double scale_factor_x, double scale_factor_y)
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
        scale_point((Point_d *)all_points[i]->object, &centroid, scale_factor_x, scale_factor_y);
    }
}

void *scale(Node_ptr o, bool is_scale_up, bool scale_x, bool scale_y)
{

    if (o == NULL || ((Node_ptr)o)->object == NULL)
    {
        perror(RED "ERROR: O objeto não foi selecionado\n" RESET);
        return NULL;
    }

    glut_post_redisplay();

    double scale_variation = 0.1;

    if (is_scale_up == true)
        scale_variation = 1 + scale_variation;
    else
        scale_variation = 1 - scale_variation;

    double scale_factor_x = (scale_x == true) ? scale_variation : 1;
    double scale_factor_y = (scale_y == true) ? scale_variation : 1;

    switch (((Node_ptr)o)->type)
    {
    case LINE_T:
        scale_line(((Node_ptr)o)->object, scale_factor_x, scale_factor_y);
        break;
    case POLYGON_T:
        scale_polygon(((Node_ptr)o)->object, scale_factor_x, scale_factor_y);
        break;
    default:
        break;
    }
}

// TODO: Convert to Convex

// Comparador para qsort
int comparate(const void *a, const void *b) {
    double arg1 = *(const double *)a;
    double arg2 = *(const double *)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

// Usado em Graham Scan
int compare_points(const void *a, const void *b) {
    Point_d *p1 = *(Point_d **)a;
    Point_d *p2 = *(Point_d **)b;
    if (p1->x == p2->x)
        return p1->y - p2->y;
    return p1->x - p2->x;
}

int orientation(Point_d *p, Point_d *q, Point_d *r) {
    int val = (q->y - p->y) * (r->x - q->x) -
              (q->x - p->x) * (r->y - q->y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

Structure *graham_scan(Point_d **points, int n) {
    if (n <= 3) {
        Structure *res = create_structure();
        for (int i = 0; i < n; i++)
            add_object(res, points[i], POINT_T);
        return res;
    }

    qsort(points, n, sizeof(Point_d *), compare_points);

    Point_d **hull = malloc(n * sizeof(Point_d *));
    int k = 0;

    for (int i = 0; i < n; i++) {
        while (k >= 2 && orientation(hull[k - 2], hull[k - 1], points[i]) != 2)
            k--;
        hull[k++] = points[i];
    }

    int t = k + 1;
    for (int i = n - 2; i >= 0; i--) {
        while (k >= t && orientation(hull[k - 2], hull[k - 1], points[i]) != 2)
            k--;
        hull[k++] = points[i];
    }

    Structure *res = create_structure();
    for (int i = 0; i < k - 1; i++)
        add_object(res, hull[i], POINT_T);

    free(hull);
    return res;
}

double calculate_median(Node_ptr *all_points, int size) {
    double *x_values = malloc(size * sizeof(double));
    if (x_values == NULL) {
        printf("Erro ao alocar memória.\n");
        return -1;
    }

    for (int i = 0; i < size; i++)
        x_values[i] = ((Point_d *)all_points[i]->object)->x;

    qsort(x_values, size, sizeof(double), comparate);

    double median;
    if (size % 2 == 0)
        median = (x_values[size / 2 - 1] + x_values[size / 2]) / 2.0;
    else
        median = x_values[size / 2];

    free(x_values);
    return median;
}

Structure *merge_polygon(Structure *left_points, Structure *right_points) {
    int total = left_points->num_objects + right_points->num_objects;
    Point_d **all = malloc(total * sizeof(Point_d *));

    Node_ptr *left_arr = get_all(left_points);
    Node_ptr *right_arr = get_all(right_points);

    for (int i = 0; i < left_points->num_objects; i++)
        all[i] = (Point_d *)left_arr[i]->object;

    for (int i = 0; i < right_points->num_objects; i++)
        all[left_points->num_objects + i] = (Point_d *)right_arr[i]->object;

    Structure *res = graham_scan(all, total);

    free(all);
    return res;
}

Structure *divide_and_conquiste(Structure *list) {
    if (list->num_objects <= 3)
        return list;

    double median = calculate_median(get_all(list), list->num_objects);
    Structure **splited_list = split_list(list, median);

    Structure *left_points = divide_and_conquiste(splited_list[0]);
    Structure *right_points = divide_and_conquiste(splited_list[1]);

    Structure *merged = merge_polygon(left_points, right_points);

    return merged;
}

void *convert_to_convex(Node_ptr o) {
    if (o == NULL || o->object == NULL) {
        perror(RED "ERROR: O objeto não foi selecionado\n" RESET);
        return NULL;
    }

    if (o->type != POLYGON_T) return NULL;

    Structure *new_points = divide_and_conquiste(((Polygon_d *)o->object)->vertices);
    ((Polygon_d *)o->object)->vertices = new_points;

    glut_post_redisplay();
    return NULL;
}