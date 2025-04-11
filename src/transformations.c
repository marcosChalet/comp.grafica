#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "utils.h"
#include "transformations.h"
#include "opengl_functions.h"

#define ANGLE 30
#define M_PI acos(-1.0)

void is_infinit_loop() {
    static int asd = 0;
    if (asd++ > 100) {
        printf("looop infinito!!!!\n");
        exit(0);
    }
}

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

int orientation(Point_d *a, Point_d *b, Point_d *c) {
    double val = (b->x - a->x) * (c->y - a->y) - (b->y - a->y) * (c->x - a->x);
    return (val > 1e-9) ? 1 : (val < -1e-9) ? -1 : 0;
}

void update_structure_metadata(Structure *pol) {
    if (!pol || !pol->head) return;
    
    Node_ptr current = pol->head;
    int count = 0;
    do {
        count++;
        current = current->next;
    } while(current != pol->head);
    
    pol->num_objects = count;
}

int compare_nodes(const void *a, const void *b) {
    Point_d *p1 = (*(Node_ptr *)a)->object;
    Point_d *p2 = (*(Node_ptr *)b)->object;

    if (fabs(p1->x - p2->x) > 1e-9) {
        return (p1->x > p2->x) * 2 - 1;
    }
    return (p1->y > p2->y) * 2 - 1;
}

void sort_structure(Structure *pol) {
    if (!pol || pol->num_objects < 2) return;

    Node_ptr *arr = malloc(pol->num_objects * sizeof(Node_ptr));
    Node_ptr current = pol->head;
    for (int i = 0; i < pol->num_objects; i++) {
        arr[i] = current;
        current = current->next;
    }

    qsort(arr, pol->num_objects, sizeof(Node_ptr), compare_nodes);

    for (int i = 0; i < pol->num_objects; i++) {
        arr[i]->next = arr[(i + 1) % pol->num_objects];
        arr[i]->prev = arr[(i - 1 + pol->num_objects) % pol->num_objects];
    }
    pol->head = arr[0];

    free(arr);
    update_structure_metadata(pol);
}

int is_convex(Structure *pol) {
    if (!pol || pol->num_objects < 3) return 0;

    Node_ptr current = pol->head;
    int sign = 0;

    do {
        Node_ptr next = current->next;
        Node_ptr next_next = next->next;

        int o = orientation(current->object, next->object, next_next->object);
        
        if (o != 0) {
            if (sign == 0) sign = o;
            else if (o != sign) return 0;
        }

        current = current->next;
    } while (current != pol->head);

    return (sign != 0);
}

Node_ptr find_right_most(Structure * pol) {
    if (!pol || !pol->head) return NULL;

    Node_ptr aux = pol->head;
    Node_ptr right_most = pol->head;

    do {
        if (((Point_d *)right_most->object)->x < ((Point_d *)aux->object)->x) {
            right_most = aux;
        }
        aux = aux->next;
    } while(aux != pol->head);
    
    return right_most;
}

Node_ptr find_left_most(Structure * pol) {
    if (!pol || !pol->head) return NULL;
    
    Node_ptr aux = pol->head;
    Node_ptr left_most = pol->head;

    do {
        if (((Point_d *)left_most->object)->x > ((Point_d *)aux->object)->x) {
            left_most = aux;
        }
        aux = aux->next;
    } while(aux != pol->head);

    return left_most;
}

Node_ptr* find_upper_tangent(Node_ptr left, Node_ptr right) {
    Node_ptr *tangent = malloc(2 * sizeof *tangent);

    tangent[0] = left;
    tangent[1] = right;
    
    int changed;
    do {
        changed = 0;

        while (orientation(tangent[0]->object,
                           tangent[1]->object,
                           tangent[0]->prev->object) > 0) {
            tangent[0] = tangent[0]->prev;
            changed = 1;
        }
        
        while (orientation(tangent[0]->object,
                           tangent[1]->object,
                           tangent[1]->next->object) > 0) {
            tangent[1] = tangent[1]->next;
            changed = 1;
        }
    } while (changed);
    
    return tangent;
}

Node_ptr* find_lower_tangent(Node_ptr leftmost, Node_ptr rightmost) {
    Node_ptr *tangent = malloc(2 * sizeof *tangent);

    tangent[0] = leftmost;
    tangent[1] = rightmost;
    
    int changed;
    do {
        changed = 0;
        
        while (orientation(tangent[0]->object,
                           tangent[1]->object,
                           tangent[0]->next->object) < 0) {
            tangent[0] = tangent[0]->next;
            changed = 1;
        }
        
        while (orientation(tangent[0]->object,
                           tangent[1]->object,
                           tangent[1]->prev->object) < 0) {
            tangent[1] = tangent[1]->prev;
            changed = 1;
        }
    } while (changed);

    return tangent;
}

Structure *merge_polygons(Structure *left_pol, Structure *right_pol) {
    Node_ptr rightmost_left = find_right_most(left_pol);
    Node_ptr leftmost_right = find_left_most(right_pol);
    
    Node_ptr * upper = find_upper_tangent(rightmost_left, leftmost_right);
    Node_ptr * lower = find_lower_tangent(rightmost_left, leftmost_right);

    upper[0]->next = upper[1];
    upper[1]->prev = upper[0];
    lower[1]->next = lower[0];
    lower[0]->prev = lower[1];
    
    left_pol->head = upper[0];
    update_structure_metadata(left_pol);
    
    free(upper);
    free(lower);
    free(right_pol);
    
    return left_pol;
}

Structure **split_at_median(Structure *pol) {
    int n = pol->num_objects;
    if (n < 2) {
        Structure **res = malloc(2 * sizeof *res);
        res[0] = copy_structure(pol);
        res[1] = create_structure();
        return res;
    }

    Node_ptr *arr = malloc(n * sizeof *arr);
    if (!arr) { perror("malloc"); exit(EXIT_FAILURE); }
    Node_ptr cur = pol->head;
    for (int i = 0; i < n; i++, cur = cur->next) {
        arr[i] = cur;
    }

    qsort(arr, n, sizeof *arr, compare_nodes);

    double median_x = ((Point_d *)arr[n/2]->object)->x;

    Structure *left  = create_structure();
    Structure *right = create_structure();
    for (int i = 0; i < n; i++) {
        Point_d *p = (Point_d *)arr[i]->object;
        if (p->x < median_x)
            add_object(left, p, arr[i]->type);
        else
            add_object(right, p, arr[i]->type);
    }

    free(arr);

    Structure **result = malloc(2 * sizeof *result);
    result[0] = left;
    result[1] = right;
    return result;
}

Structure *divide_and_conquer_alg(Structure *polygon) {
    if (polygon->num_objects <= 3) {
        return polygon;
    }

    if (is_convex(polygon)) {
        printf("Encontrou um polígono convexo!\n");
        return polygon;
    }
    
    Structure **split = split_at_median(polygon);

    Structure *left = divide_and_conquer_alg(split[0]);
    Structure *right = divide_and_conquer_alg(split[1]);
    Structure *merged = merge_polygons(left, right);

    return merged;
}

void divide_and_conquer(Node_ptr selected_node) {
    if (selected_node->type != POLYGON_T) return;

    Node_ptr new_node = malloc(sizeof(Node));
    new_node->object = malloc(sizeof(Polygon_d));

    ((Polygon_d*)new_node->object)->vertices = copy_structure(((Polygon_d *)selected_node->object)->vertices);

    sort_structure(((Polygon_d *)new_node->object)->vertices);

    Structure * new_vertices = divide_and_conquer_alg(
        ((Polygon_d *)new_node->object)->vertices
    );

    ((Polygon_d *)selected_node->object)->vertices = new_vertices;
}