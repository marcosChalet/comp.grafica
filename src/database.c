#include <stdio.h>
#include <stdlib.h>
#include "opengl_functions.h"
#include "database.h"
#include "events.h"
#include "utils.h"

void load_point(FILE *, Structure *);

void load_polygon(FILE * fp, Structure * objects) {
    int num_objects;
    Structure * vertices = create_structure();

    if (!fscanf(fp, "%d\n", &num_objects)) {
        perror(RED "Error: Não foi possível ler o polígono" RESET);
        return;
    }

    for (int i = 0; i < num_objects; i++) {
        load_point(fp, vertices);
    }

    add_object(objects, object_factory(vertices, POLYGON_T), POLYGON_T);
}

void load_line(FILE * fp, Structure * objects) {
    Point_d sp, ep;
    Line_d * l = malloc(sizeof(Line_d));

    l->s_point = malloc(sizeof(Point_d));
    verify_allocation_error(l->s_point);

    l->e_point = malloc(sizeof(Point_d));
    verify_allocation_error(l->e_point);

    if (!fscanf(fp, "%d,%d,%d,%d\n",
            &(l->s_point->x),
            &(l->s_point->y),
            &(l->e_point->x),
            &(l->e_point->y)
        )
    ) {
        perror(RED "Error: Não foi possível ler a linha" RESET);
    }
    
    add_object(objects, object_factory(l, LINE_T), LINE_T);
}

void load_point(FILE * fp, Structure * objects) {
    Point_d p;

    if (!fscanf(fp, "%d,%d\n", &(p.x), &(p.y))) {
        perror(RED "Error: Não foi possível ler o ponto" RESET);
        return;
    }

    add_object(objects, object_factory(&p, POINT_T), POINT_T);
}

void save_polygon(FILE * fp, Polygon_d * p) {
    int num_objects = get_num_objects(p->vertices);
    Node_ptr * all_objects = get_all(p->vertices);
    fprintf(fp, "%d\n", POLYGON_T);
    fprintf(fp, "%d\n", num_objects);
    for (int i = 0; i < num_objects; i++) {
        fprintf(fp, "%d,%d\n", 
            ((Point_d*)all_objects[i]->object)->x,
            ((Point_d*)all_objects[i]->object)->y
        );
    }
}

void save_line(FILE * fp, Line_d * l) {
    fprintf(fp, "%d\n", LINE_T);
    fprintf(fp, "%d,%d,%d,%d\n", l->s_point->x, l->s_point->y, l->e_point->x, l->e_point->y);
}

void save_point(FILE * fp, Point_d * p) {
    fprintf(fp, "%d\n", POINT_T);
    fprintf(fp, "%d,%d\n", p->x, p->y);
}

void load_objects(Structure * objects) {
    int type;
    FILE * fp = fopen(FILENAME, "r");
    if (file_error(fp)) return;

    while(fscanf(fp, "%d\n", &type) != EOF) {
        switch (type) {
            case POINT_T   : load_point(fp, objects); break;
            case LINE_T    : load_line(fp, objects); break;
            case POLYGON_T : load_polygon(fp, objects); break;
        }
    }

    printf(GREEN "SUCCESS: Objetos carregados com sucesso\n\n" RESET);
    fclose(fp);

    draw_objects();
    glut_post_redisplay();
}

void save_objects(Structure * objects) {
    Node_ptr * all_objects = get_all(objects);
    FILE * fp = fopen(FILENAME, "w");
    if (file_error(fp));

    int num_objects = get_num_objects(objects);

    for(int i = 0; all_objects[i] != NULL; i++) {
        switch (all_objects[i]->type) {
            case POINT_T   : save_point(fp, all_objects[i]->object); break;
            case LINE_T    : save_line(fp, all_objects[i]->object); break;
            case POLYGON_T : save_polygon(fp, all_objects[i]->object); break;
        }
    }

    printf(GREEN "SUCCESS: Objetos salvos com sucesso\n\n" RESET);
    fclose(fp);
}