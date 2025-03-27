/* Código escrito por João Edison Roso Manica */
#include <stdio.h>
#include <stdlib.h>
#include "../base_structures/linkedlist.c"
#include "../base_structures/avltree.c"

#define MAX_BLOCKS 3000
#define DISTANCE 64
#define TO_1D(X,Y)\
    ((X) * max_d) + Y

int x,y;
int max_d;

void read_data(blocks, startx, starty)
avl_tree *blocks;
int *startx, *starty;
{
    size_t size;
    char *line;
    int *new_block;
    int new_blocks[MAX_BLOCKS][2];
    int new_blocks_end = 0;
    
    y = size = 0;
    line = NULL;
    while (getline(&line, &size, stdin) > 2) {
        for (x=0; line[x] != '\n'; x++)
            if (line[x] == '#') {
                new_blocks[new_blocks_end  ][0] = x;
                new_blocks[new_blocks_end++][1] = y;
                assert(new_blocks_end < MAX_BLOCKS);
            } else if (line[x] == 'S') {
                *startx = x;
                *starty = y;
            }
        y++;
    }
    free(line);
    max_d = x > y? x : y;
    while (new_blocks_end--) {
        new_block = malloc(sizeof(int));
        *new_block = TO_1D(new_blocks[new_blocks_end][0], new_blocks[new_blocks_end][1]);
        insert_avltree(blocks, new_block, NULL);
    }
}

typedef struct {
    int i,j,steps;
} path;

comp_coordinate(c1, c2)
void *c1, *c2;
{
    return *(int*)c1 - *(int*)c2;
}

traverse(blocks, startx, starty)
avl_tree *blocks;
{
    path *p, curr, *new_path;
    list_linkedlist q = { 0 };
    avl_tree visited;
    int coord_1d, *new_location, counter;
       
    create_alvtree(visited, comp_coordinate);

    curr.i = startx;
    curr.j = starty;
    new_location = malloc(sizeof(int));
    *new_location = TO_1D(curr.i, curr.j);
    insert_avltree(&visited, new_location, NULL);

    counter = curr.steps = 0;
    new_path = malloc(sizeof(path));
    memcpy(new_path, &curr, sizeof(path));
    append_linkedlist(&q, new_path);
    while ((p = pop_linkedlist(&q))) {
        coord_1d = TO_1D(p->i, p->j);
        if (p->steps > DISTANCE) {
            free(p);
            continue;
        }
        curr.steps = p->steps + 1;
        if ((p->i + p->j) % 2 == DISTANCE % 2)
            counter++;            
        if (p->j < y-1) {
            curr.i = p->i;
            curr.j = p->j+1;
            coord_1d = TO_1D(curr.i, curr.j);
            if (!(find_node_avltree_ptr(blocks, &coord_1d) || find_node_avltree(visited, &coord_1d))) {
                new_location = malloc(sizeof(int));
                *new_location = coord_1d;
                insert_avltree(&visited, new_location, NULL);
                new_path = malloc(sizeof(path));
                memcpy(new_path, &curr, sizeof(path));
                append_linkedlist(&q, new_path);
            }
        }
        if (p->i < x-1) {
            curr.i = p->i+1;
            curr.j = p->j;
            coord_1d = TO_1D(curr.i, curr.j);
            if (!(find_node_avltree_ptr(blocks, &coord_1d) || find_node_avltree(visited, &coord_1d))) {
                new_location = malloc(sizeof(int));
                *new_location = coord_1d;
                insert_avltree(&visited, new_location, NULL);
                new_path = malloc(sizeof(path));
                memcpy(new_path, &curr, sizeof(path));
                append_linkedlist(&q, new_path);
            }
        }
        if (p->i) {
            curr.i = p->i-1;
            curr.j = p->j;
            coord_1d = TO_1D(curr.i, curr.j);
            if (!(find_node_avltree_ptr(blocks, &coord_1d) || find_node_avltree(visited, &coord_1d))) {
                new_location = malloc(sizeof(int));
                *new_location = coord_1d;
                insert_avltree(&visited, new_location, NULL);
                new_path = malloc(sizeof(path));
                memcpy(new_path, &curr, sizeof(path));
                append_linkedlist(&q, new_path);
            }
        }
        if (p->j) {
            curr.i = p->i;
            curr.j = p->j-1;
            coord_1d = TO_1D(curr.i, curr.j);
            if (!(find_node_avltree_ptr(blocks, &coord_1d) || find_node_avltree(visited, &coord_1d))) {
                new_location = malloc(sizeof(int));
                *new_location = coord_1d;
                insert_avltree(&visited, new_location, NULL);
                new_path = malloc(sizeof(path));
                memcpy(new_path, &curr, sizeof(path));
                append_linkedlist(&q, new_path);
            }
        }
        free(p);
    }
    destroy_avltree(visited);
    return counter;
}

main()
{
    avl_tree blocks;
    int startx, starty;

    create_alvtree(blocks, comp_coordinate);
    read_data(&blocks, &startx, &starty);
    printf("%d\n", traverse(&blocks, startx, starty));
    destroy_avltree(blocks);
}
