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

#include "avltree.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void destroy_avl(t, r)
avl_tree *t;
avl_node *r;
{
    if (!r)
        return;
    destroy_avl(t, r->child[0]);
    destroy_avl(t, r->child[1]);
    free(r->key);
    if (r->has_value) free(r->value);
    free(r);
}

avl_node *find_max_avltree(t, r)
avl_tree *t;
avl_node *r;
{
    if (!r) return NULL;
    if (r->child[1])
        return find_max_avltree(t, r->child[1]);
    return r;
}

avl_node *find_node_avl(t, r, key, parent)
avl_tree *t;
avl_node *r, **parent;
void *key;
{
    int cmp;
    
    if (!r) return NULL;
    if (!(cmp = t->compar(key, r->key)))
        return r;
    if (parent)
        *parent = r;
    return find_node_avl(t, r->child[cmp > 0], key, parent);
}

// Least mirroed alg.
static void retrace(t, x, z, inc, removed)
avl_tree *t;
avl_node *x, *z;
unsigned char removed;
{
    int orig_bf, bf_z0, bf_y0;
    unsigned char side_x;
    avl_node *y;

    if (!x)
        return;

    orig_bf = x->bf;
    x->bf += inc;
    
    // Apply rotations.
    if (x->parent)
        side_x = x->parent->child[1] == x;
    // R
    if (x->bf/2 > 0) {
        bf_z0 = z->bf;
        // R
        if (bf_z0 >= 0) {
            if (x->parent)
                x->parent->child[side_x] = z;
            else
                t->root = z;
            z->parent = x->parent;
            x->child[1] = z->child[0];
            if (z->child[0])
                z->child[0]->parent = x;
            z->child[0] = x;
            x->parent = z;
            
            // Update BFs.
            x->bf = -bf_z0 + 1;
            z->bf = bf_z0 - 1;
            // New x:
            x = z;
        // L    
        } else {
            y = z->child[0];
            if (x->parent)
                x->parent->child[side_x] = y;
            else
                t->root = y;
            y->parent = x->parent;
            x->child[1] = y->child[0];
            if (y->child[0])
                y->child[0]->parent = x;
            y->child[0] = x;
            x->parent = y;
            z->child[0] = y->child[1];
            if (y->child[1])
                y->child[1]->parent = z;
            y->child[1] = z;
            z->parent = y;
            
            // Update BFs.
            bf_y0 = y->bf;
            if (bf_y0 < 0) {
                x->bf = 0;
                z->bf = 1;
            } else {
                x->bf = -bf_y0;    
                z->bf = 0;
            }
            y->bf = 0;
            // New x:
            x = y;
        }
    // L
    } else if (x->bf/2 < 0) {
        bf_z0 = z->bf;
        // R    
        if (bf_z0 > 0) {
            y = z->child[1];
            if (x->parent)
                x->parent->child[side_x] = y;
            else
                t->root = y;
            y->parent = x->parent;
            x->child[0] = y->child[1];
            if (y->child[1])
                y->child[1]->parent = x;
            y->child[1] = x;
            x->parent = y;
            z->child[1] = y->child[0];
            if (y->child[0])
                y->child[0]->parent = z;
            y->child[0] = z;
            z->parent = y;
            
            // Update BFs.
            bf_y0 = y->bf;
            if (bf_y0 > 0) {
                x->bf = 0;
                z->bf = -1;
            } else {
                x->bf = -bf_y0;    
                z->bf = 0;
            }
            y->bf = 0;
            // New x:
            x = y;
        // L
        } else {
            if (x->parent)
                x->parent->child[side_x] = z;
            else
                t->root = z;
            z->parent = x->parent;
            x->child[0] = z->child[1];
            if (z->child[1])
                z->child[1]->parent = x;
            z->child[1] = x;
            x->parent = z;
            
            // Update BFs.
            x->bf = -bf_z0 - 1;
            z->bf = bf_z0 + 1;
            // New x:
            x = z;
        }
    }
    // If it balanced, it means that the increment or decrement was canceled
    // out for the ancestors.
    if (removed ^ (x->bf != 0)) {
        inc = abs(x->bf - orig_bf);
        retrace(t, x->parent, x->parent? x->parent->child[removed? x->parent->bf > 0: side_x] : x,
                removed ^ side_x? inc : -inc, removed);
    }
}

void insert_avltree(t, key, value)
avl_tree *t;
void *key, *value;
{
    avl_node *node, *parent, *new;
    int gt;
    
    // Commom insert in bst. ****
    parent = NULL;
    if ((node = find_node_avl(t, t->root, key, &parent))) {
        if (node->has_value) {
            node->has_value = 0;
            free(node->value);
        }
        if (value) {
            node->has_value = 1;
            node->value = value;
        }
        free(key);
        return;
    }
    if (parent) {
        gt = t->compar(key, parent->key) > 0;
        new = parent->child[gt] = malloc(sizeof(avl_node));
    } else
        new = t->root = malloc(sizeof(avl_node));
    new->parent = parent;
    new->key = key;
    if (value) {
        new->value = value;
        new->has_value = 1;
    } else
        new->has_value = 0;
    new->child[0] = new->child[1] = NULL;
    new->bf = 0;
    t->nmemb++;
    // ****
    if (parent)
        retrace(t, parent, new, gt * 2 - 1, 0);
}

remove_avltree(t, key, value)
avl_tree *t;
void *key, **value;
{
    avl_node *z, *y, *q, *parenty;
    unsigned char side_z, two;
    int inc;
    
    two = 0;
    q = NULL;
    if (!(z = find_node_avl(t, t->root, key, &q)))
        return 1;
    if (z->child[0] && z->child[1]) {
        y = find_max_avltree(t, z->child[0]);
        parenty = y->parent;
        if (parenty != z) {
            parenty->child[1] = y->child[0];
            if (y->child[0])
                y->child[0]->parent = parenty;
            y->child[0] = z->child[0];
            z->child[0]->parent = y;
        }
        y->child[1] = z->child[1];
        z->child[1]->parent = y;
        two = 1;
        // Update Y BF
        y->bf = z->bf;
    } else
        y = z->child[0]? z->child[0] : z->child[1];
    if (q) {
        side_z = q->child[1] == z;
        q->child[side_z] = y;    
    } else
        t->root = y;
    if (y)
        y->parent = q;
    
    if (z->has_value)
        free(z->value);
    free(z->key);
    free(z);
    t->nmemb--;

    // Update BFs
    if (y && two) {
        if (parenty == z) {
            parenty = y;
            inc = 1;
        } else
            inc = -1;
        retrace(t, parenty, parenty->child[parenty->bf > 0], inc, 1);
    } else if (q)
        retrace(t, q, q->child[!(q->bf || side_z) ||
                q->bf > 0],
                side_z? -1 : 1, 1);
    return 0;
}

// Printing test routines, from BEE 1201:

void infix_avl(t, r, last)
avl_tree *t;
avl_node *r, *last;
{
    if (!r)
        return;
    infix_avl(t, r->child[0], last);
    printf("%p %d [%d] (%p, %p) ^%p", r, *(int*)r->key, r->bf, r->child[0], r->child[1], r->parent);
    if (last != r)
        putchar('\n');
    infix_avl(t, r->child[1], last);
}

void prefix_avl(t, r)
avl_tree *t;
avl_node *r;
{
    if (!r)
        return;
    if (r != t->root)
        putchar('\n');
    printf("%p %d [%d] (%p, %p) ^%p", r, *(int*)r->key, r->bf, r->child[0], r->child[1], r->parent);
    prefix_avl(t, r->child[0]);
    prefix_avl(t, r->child[1]);
}

void posfix_avl(t, r)
avl_tree *t;
avl_node *r;
{
    if (!r)
        return;
    posfix_avl(t, r->child[0]);
    posfix_avl(t, r->child[1]);
    printf("%p %d [%d] (%p, %p) ^%p", r, *(int*)r->key, r->bf, r->child[0], r->child[1], r->parent);
    if (r != t->root)
        putchar('\n');
}

/* Assumes that arr has sufficient capacity. */
void keys_to_array_avl(t, r, arr, size, nmemb, capacity)
avl_tree *t;
avl_node *r;
unsigned char *arr;
size_t size;
int *nmemb;
{
    if (!r)
        return;
    keys_to_array_avl(t, r->child[0], arr, size, nmemb, capacity);
    assert(*nmemb < capacity);
    memcpy(&arr[size**nmemb], r->key, size);
    ++*nmemb;
    keys_to_array_avl(t, r->child[1], arr, size, nmemb, capacity);
}

// Debug functions:

height_avl(t, r)
avl_tree *t;
avl_node *r;
{
    int lh, rh;
    if (!r)
        return 0;
    lh = height_avl(t, r->child[0])+1;
    rh = height_avl(t, r->child[1])+1;
    if (r->bf != rh - lh || abs(r->bf) >= 2) {
        fprintf(stderr, "Node %d rh: %d lh: %d, BF -> %d\n", *(int*)r->key, rh, lh, r->bf);
        exit(1);
    }
    return rh > lh? rh : lh;
}
