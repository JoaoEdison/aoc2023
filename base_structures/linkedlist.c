/*
    Doubly linked list implementation in C.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    void *value;
    struct node *next, *prev;
} node_linkedlist;

typedef struct {
    node_linkedlist *head, *tail;
    int nmemb;
} list_linkedlist;

void append_linkedlist(l, item)
list_linkedlist *l;
void *item;
{
    node_linkedlist *new;
    
    new = malloc(sizeof(node_linkedlist));
    new->value = item;
    new->next = NULL;
    if (l->head) {
        l->tail->next = new;
        new->prev = l->tail;
    } else {
        l->head = new;
        new->prev = NULL;
    }
    l->tail = new;
    l->nmemb++;
}

void appendlloc_linkedlist(l, item, size)
list_linkedlist *l;
void *item;
size_t size;
{
    node_linkedlist *new;
    
    new = malloc(sizeof(node_linkedlist));
    new->value = malloc(size);
    memcpy(new->value, item, size);
    new->next = NULL;
    if (l->head) {
        l->tail->next = new;
        new->prev = l->tail;
    } else {
        l->head = new;    
        new->prev = NULL;
    }
    l->tail = new;
    l->nmemb++;
}

void *find_linkedlist(l, mock, compar)
list_linkedlist *l;
void *mock;
int (*compar)(const void *, const void *);
{
    node_linkedlist *next;
    
    for (next=l->head;next;next=next->next)
        if (!compar(next->value, mock))
            return next->value;
    return NULL;
}

void *pop_linkedlist(l)
list_linkedlist *l;
{
    node_linkedlist *h;
    void *item;
    
    item = NULL;
    if ((h = l->head)) {
        item = h->value;
        l->head = h->next;
        if (h->next)
            l->head->prev = NULL;
        l->nmemb--;
        free(h);
    }
    return item;
}

node_linkedlist *delete_item_linkedlist(l, item)
list_linkedlist *l;
node_linkedlist *item;
{
    node_linkedlist *next;
    
    next = item->next;    
    if (item->prev)
        item->prev->next = item->next;
    else
        l->head = item->next;
    if (item->next)
        item->next->prev = item->prev;
    else
        l->tail = item->prev;
    l->nmemb--;
    free(item);
    return next;
}

/* CAUTION: Assumes that arr has sufficient capacity. */
to_array_linkedlist(arr, l, size)
unsigned char *arr;
list_linkedlist *l;
size_t size;
{
    node_linkedlist *next;
    int i;
    
    for (i=0, next=l->head;next;next=next->next, i++)
        memcpy(&arr[i*size], next->value, size);
    return i;
}
