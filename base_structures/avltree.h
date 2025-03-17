/*
    AVL tree implementation in C.
    Copyright (C) 2025  João Manica

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef H_AVLTREE
#define H_AVLTREE
#include <stdio.h>

typedef struct avl_node {
    short bf;
    struct avl_node *parent, *child[2];
    unsigned char has_value;
    void *key, *value;
} avl_node;

typedef struct {
    struct avl_node *root;
    int nmemb;
    int (*compar)(void *, void *);
} avl_tree;

#define create_alvtree(T, CMP_FN) \
	do { T.root = NULL; T.nmemb = 0; T.compar = CMP_FN; } while (0)

void destroy_avl(avl_tree *t, avl_node *r);
#define destroy_avltree(T) \
    destroy_avl(&T, T.root)

avl_node *find_node_avltree(avl_tree *t, avl_node *r, void *key, avl_node **parent);
avl_node *find_max_avltree(avl_tree *t, avl_node *r);
void insert_avltree(avl_tree *t, void *key, void *value);
int remove_avltree(avl_tree *t, void *key, void **value);

int height_avl(avl_tree *t, avl_node *r);
#define height_avltree(T) \
    height_avl(&T, T.root)

void infix_avl(avl_tree *t, avl_node *r, avl_node *last);
#define infix_avltree(T) \
    infix_avl(&T, T.root, find_max_avltree(&T, T.root))

void prefix_avl(avl_tree *t, avl_node *r);
#define prefix_avltree(T) \
    prefix_avl(&T, T.root)

void posfix_avl(avl_tree *t, avl_node *r);
#define posfix_avltree(T) \
    posfix_avl(&T, T.root)

void keys_to_array_avl(avl_tree *t, avl_node *r, unsigned char *arr, size_t size, int *nmemb, int capacity);
#define keys_to_array_avltree(T, ARR, SIZE, NMEMB, CAP) \
    keys_to_array_avl(&T, T.root, ARR, SIZE, NMEMB, CAP)

#endif
