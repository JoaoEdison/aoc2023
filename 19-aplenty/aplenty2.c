/* Código escrito por João Edison Roso Manica */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../base_structures/linkedlist.c"
#include "../base_structures/avltree.c"
#include <assert.h>

#define DIM 4

#define TABLE_SIZE 51
#define ARRINTERSIZE 1000
#define STACK_SIZE 20

typedef unsigned char positive;

struct workflow {
    // first id is from the node.
    char names[DIM+2][5];
    // Compared number.
    short rules[DIM];
    // next node if rule is true.
    struct workflow *next[DIM+1];
    // greater is positive if the rule is for greater.
    // numcat is the number of rules.
    // type is not zero if it goes to another node.
    positive greater,type,numcat,categories[DIM];
};

hashstr(s)
char *s;
{
    int res;
    
    for (res=0;*s;s++)
        res += *s;    
    return res%TABLE_SIZE;
}

compare_workflows(w1, w2)
const void *w1, *w2;
{
    return strcmp(((struct workflow*)w1)->names[0], ((struct workflow*)w2)->names[0]);
}

struct workflow *new;
char bufferin[50];

// Interpret rule.
void rules()
{
    char *c,*p;
    positive cat;
    
    for (c=bufferin;*c;c++){
        if (*c=='x')
            cat=0;
        else if (*c=='m')
            cat=1;
        else if (*c=='a')
            cat=2;
        else if (*c=='s')
            cat=3;
        else if (*c=='>'||*c=='<') {
            new->greater |= (*c=='>')<<new->numcat;
            new->categories[new->numcat] = cat;
            new->rules[new->numcat++] = strtol(c+1, &c, 0);
            for (c++,p=new->names[new->numcat];*c!=',';)
                *p++ = *c++;
            *p = '\0';
        }
    }
    for (;*c!=',';c--);
    c++;
    strcpy(new->names[new->numcat+1],c);
}

// Add rules to hash table.
void parse(table)
list_linkedlist table[];
{
    positive inside,first;
    char c,*p;
    
    first=1;
    inside=0;
    new = malloc(sizeof(struct workflow));
    new->type=new->greater=new->numcat=0;
    p = new->names[0];
    while ((c = getchar())!='\n'){
        if (c == '{')
            first=inside=1;
        else if (c == '}') {
            *p = '\0';
            rules();
            getchar();
            new = malloc(sizeof(struct workflow));
            new->type=new->greater=new->numcat=0;
            p = new->names[0];
        } else if (inside) {
            if (first) {
                *p = '\0';
                append_linkedlist(&table[hashstr(new->names[0])], new);
                first=0;
                p = bufferin;
                *p++ = c;
            } else
                *p++ = c;
        } else
            *p++ = c;
    }
    free(new);
}

// Create the shortcut pointers in the hash table.
void assemble(table)
list_linkedlist table[];
{
    int i,k;
    node_linkedlist *next;
    struct workflow *curr, mock;
        
    for (i=0;i<TABLE_SIZE;i++)
        for (next=table[i].head;next;next=next->next) {
            curr = next->value;
            for (k=0;k<=curr->numcat;k++)
                if (curr->names[k+1][0] == 'A')
                    curr->next[k] = NULL;
                else if (curr->names[k+1][0] == 'R')
                    curr->next[k] = 1;
                else {
                    curr->type |= 1<<k;
                    strcpy(mock.names[0], curr->names[k+1]);
                    curr->next[k] = find_linkedlist(&table[hashstr(mock.names[0])], &mock, compare_workflows);
                }
        }
}

// Gets all ranges that are accepted.
void groupings(work_ini, inter, accepted, end_accepted)
struct workflow *work_ini;
short inter[], *accepted;
int *end_accepted;
{
    short arr[DIM*2], aux;
    struct workflow *work;
    int i,j;
    
    struct stack_iter {
        short inter[DIM*2];
        struct workflow *work;
    } *stack;
    int stack_end = 0;

    stack = malloc(sizeof(struct stack_iter) * STACK_SIZE);
    memcpy(stack[0].inter, inter, sizeof(short)*DIM*2);
    stack[0].work = work_ini;
    stack_end++;

    while (stack_end--) {
        work = stack[stack_end].work;
        memcpy(arr, stack[stack_end].inter, sizeof(short)*DIM*2);

        for (i=0;i<work->numcat;i++) {
            j = work->categories[i];
            
            // end of a branch
            assert(arr[j*2] <= arr[j*2+1] && arr[j*2]);

            if (work->greater & 1<<i) {
                aux = arr[j*2];
                arr[j*2] = work->rules[i]+1;
            } else {
                aux = arr[j*2+1];
                arr[j*2+1] = work->rules[i]-1;
            }
            if (work->type & 1<<i) {
                memcpy(stack[stack_end].inter, arr, sizeof(short)*DIM*2);
                stack[stack_end].work = work->next[i];
                assert(++stack_end < STACK_SIZE);
            } else if (!work->next[i]){
                memcpy(&accepted[*end_accepted*DIM*2], arr, sizeof(short)*DIM*2);
                assert(++*end_accepted < ARRINTERSIZE);
            }
            // will check the others rules
            if (work->greater & 1<<i) {
                arr[j*2] = aux;
                arr[j*2+1] = work->rules[i];
            } else {
                arr[j*2] = work->rules[i];
                arr[j*2+1] = aux;
            }
        }
        if (work->type & 1<<i) {
            memcpy(stack[stack_end].inter, arr, sizeof(short)*DIM*2);
            stack[stack_end].work = work->next[i];
            assert(++stack_end < STACK_SIZE);
        } else if (!work->next[i]){
            memcpy(&accepted[*end_accepted*DIM*2], arr, sizeof(short)*DIM*2);
            assert(++*end_accepted < ARRINTERSIZE);
        }
    }
    free(stack);
}

// Merge intervals if possible.
void new_comb(interval, unified)
short *interval;
list_linkedlist *unified;
{
    positive contained, contains, diff, diff_interval;
    node_linkedlist *next;
    short *comp;
    int j;
    
    for (next=unified->head;next;) {
        comp = next->value;
        contains = 1;
        for (j=0;j<7;j+=2)
            if (interval[j+1]>comp[j+1]||interval[j]<comp[j]) {
                contains = 0;
                break;
            }
        if (contains)
            return;
        contained = 1;
        for (j=0;j<7;j+=2)
            if (interval[j+1]<comp[j+1]||interval[j]>comp[j]) {
                contained = 0;
                break;
            }
        if (contained) {
            next = delete_item_linkedlist(unified, next);
            continue;
        }
        diff = 0;
        for (j=0;j<7;j+=2)
            if (interval[j+1]!=comp[j+1]||interval[j]!=comp[j]) {
                diff_interval = j;
                if (++diff > 1)
                    break;
            }
        if (diff == 1) {
            j = diff_interval;
            // [*****[-----]-----]
            //  interval  comp
            if (interval[j+1]+1 >= comp[j] && interval[j] < comp[j]) {
                interval[j+1] = comp[j+1];
                next = delete_item_linkedlist(unified, next);
                continue;
            }
            // [-----[*****]*****]
            //  comp  interval
            if (interval[j] <= comp[j+1]+1 && interval[j+1] > comp[j+1]) {
                interval[j] = comp[j];
                next = delete_item_linkedlist(unified, next);
                continue;
            }
        }
        next=next->next;
    }
    appendlloc_linkedlist(unified, interval, sizeof(short)*DIM*2);
}

compare_limits(l1, l2)
void *l1, *l2;
{
    return *(short*)l1-*(short*)l2;
}

// Original algorithm get from ChatGPT in Python, but very very changed.
// Counts unique interval combinations with overlaps:
// Needs to be reprogrammed if DIM != 4.
// TODO: see if using interval trees makes faster.
unsigned long long uniq_inter_comb(intervals, end)
short *intervals;
{
    avl_tree bounds[DIM];
    short *bounds_uniq_sorted[DIM];
    int end_bounds_uniq_sorted[DIM] = { 0 };
    short *key, x1, x2, y1, y2, z1, z2, w1, w2, dx, dy, dz, dw;
    int i, j, k, l, m;
    unsigned long long total;
    
    for (i=0; i < DIM; i++) {
        create_alvtree(bounds[i], compare_limits);
        bounds_uniq_sorted[i] = malloc(sizeof(short) * end);
    }
    for (i=0; i < end; i++)
        for (j=0; j < DIM; j++) {
            key = malloc(sizeof(short));
            *key = intervals[i*DIM*2+j*2];
            insert_avltree(&bounds[j], key, NULL);
            key = malloc(sizeof(short));
            *key = intervals[i*DIM*2+j*2+1]+1;
            insert_avltree(&bounds[j], key, NULL);
        }
    for (i=0; i < DIM; i++)
        keys_to_array_avltree(bounds[i], (void*)bounds_uniq_sorted[i], sizeof(short), &end_bounds_uniq_sorted[i], end);
    
    struct {
        int *interval_idx;
        int nmemb; 
    } stack_x, stack_y, stack_z;

    total = 0;
    stack_x.interval_idx = malloc(sizeof(int)*end);
    stack_y.interval_idx = malloc(sizeof(int)*end);
    stack_z.interval_idx = malloc(sizeof(int)*end);
    for (i=0; i < end_bounds_uniq_sorted[0] - 1; i++) {
        x1 = bounds_uniq_sorted[0][i];
        x2 = bounds_uniq_sorted[0][i+1];
        dx = x2 - x1;
        // Only intervals that cover x1 and x2 from all intervals.
        stack_x.nmemb = 0;
        for (m=0; m < end; m++)
            if (intervals[m*8+0] <= x1 && intervals[m*8+1] >= x2 - 1)
                stack_x.interval_idx[stack_x.nmemb++] = m;
        for (j=0; j < end_bounds_uniq_sorted[1] - 1; j++) {
            y1 = bounds_uniq_sorted[1][j];
            y2 = bounds_uniq_sorted[1][j+1];
            dy = y2 - y1; 
            // Only intervals that cover y1 and y2 from stack_x.
            stack_y.nmemb = 0;
            for (m=0; m < stack_x.nmemb; m++)
                if (intervals[stack_x.interval_idx[m]*8+2] <= y1 &&
                    intervals[stack_x.interval_idx[m]*8+3] >= y2 - 1)
                    stack_y.interval_idx[stack_y.nmemb++] = stack_x.interval_idx[m];
            for (k=0; k < end_bounds_uniq_sorted[2] - 1; k++) {
                z1 = bounds_uniq_sorted[2][k];
                z2 = bounds_uniq_sorted[2][k+1];
                dz = z2 - z1; 
                // Only intervals that cover z1 and z2 from stack_y.
                stack_z.nmemb = 0;
                for (m=0; m < stack_y.nmemb; m++)
                    if (intervals[stack_y.interval_idx[m]*8+4] <= z1 &&
                        intervals[stack_y.interval_idx[m]*8+5] >= z2 - 1)
                        stack_z.interval_idx[stack_z.nmemb++] = stack_y.interval_idx[m];
                for (l=0; l < end_bounds_uniq_sorted[3] - 1; l++) {
                    w1 = bounds_uniq_sorted[3][l];
                    w2 = bounds_uniq_sorted[3][l+1];
                    dw = w2 - w1; 
                    for (m=0; m < stack_z.nmemb; m++)
                        if (intervals[stack_z.interval_idx[m]*8+6] <= w1 &&
                            intervals[stack_z.interval_idx[m]*8+7] >= w2 - 1) {
                            total += (unsigned long long)dx * dy * dz * dw;
                            break;
                        }
                }
            }
        }
    }
    free(stack_x.interval_idx);
    free(stack_y.interval_idx);
    free(stack_z.interval_idx);
    for (i=0; i < DIM; i++) {
        destroy_avltree(bounds[i]);
        free(bounds_uniq_sorted[i]);
    }
    return total;
}

main()
{
    list_linkedlist table[TABLE_SIZE] = { 0 };
    list_linkedlist unified = { 0 };
    short *arrinter, *unarr;
    int endarr, endunarr;

    short arr[DIM*2] = {1,4000,1,4000,1,4000,1,4000};

    struct workflow mock;
    int i;

    endarr=endunarr=0;
    unarr = malloc(sizeof(short)*DIM*2*ARRINTERSIZE);
    arrinter = malloc(sizeof(short)*DIM*2*ARRINTERSIZE);

    strcpy(mock.names[0], "in");
    parse(table);
    assemble(table);
    groupings(find_linkedlist(&table[hashstr("in")], &mock, compare_workflows), arr, arrinter, &endarr);
    // unify intervals
    for (i=0;i<endarr;i++)
        new_comb(&arrinter[i*DIM*2], &unified);
    free(arrinter);
    endunarr = to_array_linkedlist(unarr, &unified, sizeof(short)*DIM*2);
    printf("%llu\n", uniq_inter_comb(unarr, endunarr));
    free(unarr);
}
