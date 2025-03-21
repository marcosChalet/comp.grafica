#include <stdio.h>
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

    add_object(objects, object_factory(vertices, POLYGON), POLYGON);
}

void load_line(FILE * fp, Structure * objects) {
    Point sp, ep;
    Line * l = malloc(sizeof(Line));

    l->s_point = malloc(sizeof(Point));
    verify_allocation_error(l->s_point);

    l->e_point = malloc(sizeof(Point));
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
    
    add_object(objects, object_factory(l, LINE), LINE);
}

void load_point(FILE * fp, Structure * objects) {
    Point p;

    if (!fscanf(fp, "%d,%d\n", &(p.x), &(p.y))) {
        perror(RED "Error: Não foi possível ler o ponto" RESET);
        return;
    }

    add_object(objects, object_factory(&p, POINT), POINT);
}

void save_polygon(FILE * fp, Polygon * p) {
    int num_objects = get_num_objects(p->vertices);
    Node_ptr * all_objects = get_all(p->vertices);
    fprintf(fp, "%d\n", POLYGON);
    fprintf(fp, "%d\n", num_objects);
    for (int i = 0; i < num_objects; i++) {
        fprintf(fp, "%d,%d\n", 
            ((Point*)all_objects[i]->object)->x,
            ((Point*)all_objects[i]->object)->y
        );
    }
}

void save_line(FILE * fp, Line * l) {
    fprintf(fp, "%d\n", LINE);
    fprintf(fp, "%d,%d,%d,%d\n", l->s_point->x, l->s_point->y, l->e_point->x, l->e_point->y);
}

void save_point(FILE * fp, Point * p) {
    fprintf(fp, "%d\n", POINT);
    fprintf(fp, "%d,%d\n", p->x, p->y);
}

void load_objects(Structure * objects) {
    int type;
    FILE * fp = fopen(FILENAME, "r");
    if (file_error(fp)) return;

    while(fscanf(fp, "%d\n", &type) != EOF) {
        switch (type) {
            case POINT   : load_point(fp, objects); break;
            case LINE    : load_line(fp, objects); break;
            case POLYGON : load_polygon(fp, objects); break;
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
            case POINT   : save_point(fp, all_objects[i]->object); break;
            case LINE    : save_line(fp, all_objects[i]->object); break;
            case POLYGON : save_polygon(fp, all_objects[i]->object); break;
        }
    }

    printf(GREEN "SUCCESS: Objetos salvos com sucesso\n\n" RESET);
    fclose(fp);
}