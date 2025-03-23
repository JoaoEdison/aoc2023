#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../base_structures/hash.c"
#include "../base_structures/math.c"

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
module *find_sender(to)
char to[];
{
    table_node *curr;
    int i, j;

    for (i=0; i < TABLE_SIZE; i++)
        for (curr=table[i]; curr; curr = curr->next)
            for (j=0; j < curr->data->num_outs; j++)
                if (!strcmp(curr->data->outs_names[j], to))
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
    char *to_name;
    char pulse;
} signal;

#define QUEUE_SIZE 60

#include "../base_structures/queue.c"
QUEUE_GENERATE(signal, QUEUE_SIZE)

int l1_qtt;
int *l2_qtt;
module **l1_watch;
long ***delay;
int **delays_end;
positive_int **last_time; 
unsigned **lambdas;
unsigned **mus;

#define MAX_QTT_INDEXES 100

positive_int all_in_cycle(i,j) {
    positive_int mmc, sum;
    int k;
    
    if (delays_end[i][j] < 2)
        return 0;
    if (!(lambdas[i][j] = brent(delay[i][j], delays_end[i][j], &mus[i][j])))
        return 0;

    for (i=0; i < l1_qtt; i++)
        for (j=0; j < l2_qtt[i]; j++)
            if (!lambdas[i][j])
                return 0;

    sum = delay[0][0][mus[0][0]];
    for (k=1; k < lambdas[0][0]; k++)
        sum += delay[0][0][mus[0][0] + k];
    mmc = sum;
    for (i=0; i < l1_qtt; i++)
        for (j=!i; j < l2_qtt[i]; j++) {
            sum = delay[i][j][mus[i][j]];
            for (k=1; k < lambdas[i][j]; k++)
                sum += delay[i][j][mus[i][j] + k];
            mmc = mmc / greatest_common_divisor(mmc, sum) * sum;
        }
    return mmc;
}

positive_int run(q, start, time)
queue *q;
module *start;
positive_int *time;
{
    positive_int mmc;
    signal curr, *ptr;
    int i, j, total_high;
    char pulse;
       
    q->start = q->end = 0;    
    curr.from = NULL;
    curr.to = start;
    curr.to_name = start->name;
    curr.pulse = LOW;
    insert_queue(q, &curr);
    ++*time;
    while ((ptr = dequeue(q))) {
        if (!ptr->to)
            continue;
        if (!ptr->pulse) {
            for (i=0; i < l1_qtt; i++)
                if (ptr->to == l1_watch[i]) {
                    // find which j is sending:
                    for (j=0; j < l2_qtt[i]; j++) {
                        if (ptr->from == l1_watch[i]->inputs_pulses[j].from &&
                            !lambdas[i][j]) {
                            delay[i][j][delays_end[i][j]] = *time - last_time[i][j];
                            last_time[i][j] = *time;
                            assert(++delays_end[i][j] < MAX_QTT_INDEXES);
                            if ((mmc = all_in_cycle(i, j)))
                                return mmc;
                            break;
                        }
                        
                    }
                    break;
                }
            //if (!(strcmp(ptr->to_name, "rx"))) 
            //    return -1;
        }
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
            curr.to_name = ptr->to->outs_names[i];
            curr.to = ptr->to->outs_ptrs[i];
            curr.from = ptr->to;
            curr.pulse = pulse;
            insert_queue(q, &curr);
        }
    }
    return 0;
}

main()
{
    queue q;
    char *line;
    size_t n;
    module *broadcaster, *conj;
    positive_int i, end_cycle;
    int j;
    
    line = NULL;
    n=0;
    while (getline(&line, &n, stdin) != EOF)
        parse_line(line);
    adjust_pointers();
    broadcaster = find("broadcaster");

    assert((conj = find_sender("rx")));
    assert(conj->type == CONJUNCTION);
    
    q.arr = malloc(sizeof(signal) * QUEUE_SIZE);

    // l0 = sender of rx.
    l1_qtt = conj->num_inputs;
    l1_watch = malloc(sizeof(input_pulse*) * l1_qtt);
    l2_qtt = malloc(sizeof(int) * l1_qtt);

    delay = malloc(sizeof(long**) * l1_qtt);

    delays_end = malloc(sizeof(int*) * l1_qtt);
    last_time = malloc(sizeof(positive_int*) * l1_qtt);
    lambdas = malloc(sizeof(unsigned*) * l1_qtt);
    mus = malloc(sizeof(unsigned*) * l1_qtt);
    for (i=0; i < l1_qtt; i++) {
        assert(conj->inputs_pulses[i].from->type == CONJUNCTION);
        l1_watch[i] = conj->inputs_pulses[i].from;
        l2_qtt[i] = conj->inputs_pulses[i].from->num_inputs;

        delay[i] = malloc(sizeof(long*) * l2_qtt[i]);

        delays_end[i] = malloc(sizeof(int) * l2_qtt[i]);
        last_time[i] = malloc(sizeof(positive_int) * l2_qtt[i]);
        lambdas[i] = malloc(sizeof(unsigned) * l2_qtt[i]);
        mus[i] = malloc(sizeof(unsigned) * l2_qtt[i]);
        for (j=0; j < l2_qtt[i]; j++) {
            delay[i][j] = malloc(sizeof(long) * MAX_QTT_INDEXES);
            last_time[i][j] = mus[i][j] = lambdas[i][j] = delays_end[i][j] = 0;
        }
    }

    i=0;
    while (!(end_cycle = run(&q, broadcaster, &i)));
    // Don't know if this result is correct, as it doesn't take into account
    // the presses before they go into cycle (the mus).
    printf("%lu\n", end_cycle);

    for (i=0; i < l1_qtt; i++) {
        for (j=0; j < l2_qtt[i]; j++)
            free(delay[i][j]);
        free(delay[i]);
        free(delays_end[i]);
        free(last_time[i]);
        free(lambdas[i]);
        free(mus[i]);
    }
    free(l1_watch);
    free(l2_qtt);
    free(delay);
    free(delays_end);
    free(last_time);
    free(lambdas);
    free(mus);
    
    free(q.arr);
    free(line);
}
