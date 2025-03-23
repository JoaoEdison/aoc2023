#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../base_structures/hash.c"

#define MAX_NEIGHBORS 20
#define MAX_NAME_SIZE 20

typedef struct module {
    char name[MAX_NAME_SIZE];
    char type;
    char outs_names[MAX_NEIGHBORS][MAX_NAME_SIZE];
    struct module *outs_ptrs[MAX_NEIGHBORS];
    int num_outs;
    struct input_pulse {
        struct module *from;
        char last_pulse;
    } inputs_pulses[MAX_NEIGHBORS];
    int num_inputs;
} module;

typedef struct input_pulse input_pulse;

typedef struct table_node {
    module *data;
    struct table_node *next;
} table_node;

#define TABLE_SIZE 10
table_node *table[TABLE_SIZE] = { 0 };

void insert_table(ptr)
module *ptr;
{
    table_node *curr, *prev;
    int i;
    
    i = hashstr(ptr->name, TABLE_SIZE);
    for (prev=NULL, curr=table[i]; curr; prev = curr, curr = curr->next);
    if (!prev) {
        table[i] = malloc(sizeof(table_node));
        table[i]->next = NULL;
        table[i]->data = ptr;
        return;
    }
    prev->next = malloc(sizeof(table_node));
    prev->next->next = NULL;
    prev->next->data = ptr;
}

#define FLIP 'f'
#define CONJUNCTION 'c'
#define BROADCAST 'b'

#define LOW 0
#define HIGH 1

void parse_line(l)
char *l;
{
    module *new;
    char *tok;
    int i;
    
    new = malloc(sizeof(module));
    if (*l == '%') {
        new->inputs_pulses[0].last_pulse = LOW;
        new->type = FLIP;
        l++;
    } else if (*l == '&') {
        for (i=0; i < MAX_NEIGHBORS; i++)
            new->inputs_pulses[i].last_pulse = LOW;
        new->type = CONJUNCTION;
        l++;
    } else
        new->type = BROADCAST;
    new->num_inputs = 0;

    tok=strtok(l, " ");
    strcpy(new->name, tok);
    i=0;
    for (tok = strtok(NULL, " -> "); tok; tok=strtok(NULL, ", ")) {
        if (tok[strlen(tok)-1] == ',' || tok[strlen(tok)-1] == '\n')
            tok[strlen(tok)-1] = '\0';
        strcpy(new->outs_names[i], tok);
        i++;
    }
    new->num_outs = i;
    insert_table(new);
}

module *find(name)
char name[];
{
    table_node *curr;
    int i;

    i = hashstr(name, TABLE_SIZE);
    for (curr=table[i]; curr; curr = curr->next)
        if (!strcmp(curr->data->name, name))
            return curr->data;
    return NULL;
}

void adjust_pointers()
{
    table_node *curr;
    module *ptr, *neigh;
    int i,j;

    for (i=0; i < TABLE_SIZE; i++)
        for (curr=table[i]; curr; curr = curr->next) {
            ptr = curr->data;
            for (j=0; j < ptr->num_outs; j++) {
                neigh = find(ptr->outs_names[j]);
                ptr->outs_ptrs[j] = neigh;
                if (neigh)
                    neigh->inputs_pulses[neigh->num_inputs++].from = ptr;
            }
        }
}

typedef struct {
    module *from, *to;
    char pulse;
} signal;

#define QUEUE_SIZE 60

#include "../base_structures/queue.c"
QUEUE_GENERATE(signal, QUEUE_SIZE)

queue q;
int total_high_pulses, total_low_pulses;

void run(start)
module *start;
{
    signal curr, *ptr;
    int i, total_high;
    char pulse;
    
    q.start = q.end = 0;
    curr.from = NULL;
    curr.to = start;
    curr.pulse = LOW;
    insert_queue(&q, &curr);
    while ((ptr = dequeue(&q))) {
        if (ptr->pulse)
            total_high_pulses++;
        else
            total_low_pulses++;
        if (!ptr->to)
            continue;
        switch (ptr->to->type) {
        case BROADCAST:
            pulse = ptr->pulse;
            break;
        case FLIP:
            if (ptr->pulse == HIGH)
                continue;
            pulse = ptr->to->inputs_pulses[0].last_pulse = !ptr->to->inputs_pulses[0].last_pulse;
            break;
        case CONJUNCTION:
            for (total_high=i=0; i < ptr->to->num_inputs; i++) {
                if (ptr->to->inputs_pulses[i].from == ptr->from)
                    ptr->to->inputs_pulses[i].last_pulse = ptr->pulse;
                if (ptr->to->inputs_pulses[i].last_pulse == HIGH)
                    total_high++;
            }
            pulse = total_high != ptr->to->num_inputs;
            break;
        }
        for (i=0; i < ptr->to->num_outs; i++) {
            curr.from = ptr->to;
            curr.to = ptr->to->outs_ptrs[i];
            curr.pulse = pulse;
            insert_queue(&q, &curr);
        }
    }
}

main()
{
    char *line;
    size_t n;
    module *broadcaster;
    int i;

    line = NULL;
    n=0;
    while (getline(&line, &n, stdin) != EOF)
        parse_line(line);
    adjust_pointers();
    q.arr = malloc(sizeof(signal) * QUEUE_SIZE);
    broadcaster = find("broadcaster");
    total_high_pulses = total_low_pulses = 0;
    for (i=0; i < 1000; i++)
        run(broadcaster);
    printf("%ld\n", ((long)total_high_pulses) * total_low_pulses);
    free(q.arr);
    free(line);
}
