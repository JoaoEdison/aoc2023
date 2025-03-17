/* Código escrito por João Edison Roso Manica */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../base_structures/linkedlist.c"

#define TABLE_SIZE 51
typedef unsigned char positive;

struct workflow {
    char names[6][5], categories[4];
    int rules[4];
    struct workflow *next[5];
    positive signal,type,numcat;
};

list_linkedlist table[TABLE_SIZE] = { 0 };

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

void rules()
{
    char cat,*c,*p;
    
    for (c=bufferin;*c;c++){
        if (*c=='x')
            cat='x';
        else if (*c=='m')
            cat='m';
        else if (*c=='a')
            cat='a';
        else if (*c=='s')
            cat='s';
        else if (*c=='>'||*c=='<') {
            new->signal |= (*c=='>')<<new->numcat;
            new->categories[new->numcat] = cat;
            new->rules[new->numcat++] = strtol(c+1, &c, 0);
            for (c++,p=new->names[new->numcat];*c!=',';)
                *p++ = *c++;
            *p = '\0';
        }
    }
    for (;*c!=',';c--);
    c++;
    strcpy(new->names[++new->numcat],c);
}
void parse()
{
    positive inside,first;
    char c,*p;
    
    first=1;
    inside=0;
    new = malloc(sizeof(struct workflow));
    new->type=new->signal=new->numcat=0;
    p = new->names[0];
    while ((c = getchar())!='\n'){
        if (c == '{')
            first=inside=1;
        else if (c == '}') {
            *p = '\0';
            rules();
            getchar();
            new = malloc(sizeof(struct workflow));
            new->type=new->signal=new->numcat=0;
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

void assemble()
{
    int i,k;
    struct node *next;
    struct workflow *curr, mock;
        
    for (i=0;i<TABLE_SIZE;i++)
        for (next=table[i].head;next;next=next->next) {
            curr = next->value;
            for (k=0;k<curr->numcat;k++)
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

evaluate(curr,arr,next)
struct workflow *curr, **next;
int arr[];
{
    int i,j;
    
    for (i=0;i<curr->numcat-1;i++) {
        if (curr->categories[i] == 'x')
            j=0;
        else if (curr->categories[i] == 'm')
            j=1;
        else if (curr->categories[i] == 'a')
            j=2;
        else
            j=3;
        if (((curr->signal & 1<<i) && arr[j] > curr->rules[i]) || 
           (!(curr->signal & 1<<i) && arr[j] < curr->rules[i])) break;
    }
    if (curr->type & 1<<i) {
        *next = curr->next[i];
        return -1;
    }
    return curr->next[i];
}

main()
{
    int xmas[4],v;
    struct workflow *in,*curr,*next,mock;
    long sum;

    parse();
    assemble();
    strcpy(mock.names[0], "in");
    in = find_linkedlist(&table[hashstr("in")], &mock, compare_workflows);
    sum=0;
    while (scanf("{x=%d,m=%d,a=%d,s=%d}\n",xmas,&xmas[1],&xmas[2],&xmas[3]) != EOF) {
        curr = in;
        while ((v = evaluate(curr,xmas,&next))<0) curr = next;
        if (!v) {
            sum+=xmas[0]; sum+=xmas[1]; sum+=xmas[2]; sum+=xmas[3];
        }
    }
    printf("%ld\n",sum);
}
