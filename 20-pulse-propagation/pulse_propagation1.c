#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NEIGHBORS 20
#define MAX_NAME_SIZE 20

struct module {
	char name[MAX_NAME_SIZE];
	char type;
	char outs_names[MAX_NEIGHBORS][MAX_NAME_SIZE];
	struct module *outs_ptrs[MAX_NEIGHBORS];
	int num_outs;
	struct {
		struct module *from;
		char pulse;
	} inputs_pulses[MAX_NEIGHBORS];
	int num_inputs;
};

typedef struct module module;

struct table_node {
	module *data;
	struct table_node *next;
};

#define TABLE_SIZE 10
struct table_node *table[TABLE_SIZE] = { 0 };

hash(s)
char s[];
{
	char *p;
	int value;

	for (value = 0, p=s; *p; p++)
		value += *p;
	return value % TABLE_SIZE;
}

void insert_table(ptr)
module *ptr;
{
	struct table_node *curr, *prev;
	int i;
	
	i = hash(ptr->name);
	for (prev=NULL, curr=table[i]; curr; prev = curr, curr = curr->next);
	if (!prev) {
		table[i] = malloc(sizeof(struct table_node));
		table[i]->next = NULL;
		table[i]->data = ptr;
		return;
	}
	prev->next = malloc(sizeof(struct table_node));
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
		new->inputs_pulses[0].pulse = LOW;
		new->type = FLIP;
		l++;
	} else if (*l == '&') {
		for (i=0; i < MAX_NEIGHBORS; i++)
			new->inputs_pulses[i].pulse = LOW;
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
	struct table_node *curr;
	int i;

	i = hash(name);
	for (curr=table[i]; curr; curr = curr->next)
		if (!strcmp(curr->data->name, name))
			return curr->data;
	return NULL;
}

void adjust_pointers()
{
	struct table_node *curr;
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

#define QUEUE_SIZE 100

signal *queue;
int queue_start, queue_end;

void insert_queue(p)
signal *p;
{
	memcpy(&queue[queue_end], p, sizeof(signal));
	queue_end = (queue_end+1) % QUEUE_SIZE;
	if (queue_end == queue_start) {
		printf("SEM ESPAÇO\n");
		exit(1);
	}
}
signal *dequeue()
{
	signal *p;
	
	p = &queue[queue_start];
	if (queue_start == queue_end)
		return NULL;
	queue_start = (queue_start+1) % QUEUE_SIZE;
	return p;
}

int total_high_pulses, total_low_pulses;

void run(start)
module *start;
{
	signal curr, *ptr;
	int i, total_high;
	char pulse;
	
	queue_start = queue_end = 0;
	curr.from = NULL;
	curr.to = start;
	curr.pulse = LOW;
	insert_queue(&curr);
	while ((ptr = dequeue())) {
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
			pulse = ptr->to->inputs_pulses[0].pulse = !ptr->to->inputs_pulses[0].pulse;
			break;
		case CONJUNCTION:
			for (total_high=i=0; i < ptr->to->num_inputs; i++) {
				if (ptr->to->inputs_pulses[i].from == ptr->from)
					ptr->to->inputs_pulses[i].pulse = ptr->pulse;
				if (ptr->to->inputs_pulses[i].pulse == HIGH)
					total_high++;
			}
			pulse = total_high != ptr->to->num_inputs;
			break;
		}
		for (i=0; i < ptr->to->num_outs; i++) {
			curr.from = ptr->to;
			curr.to = ptr->to->outs_ptrs[i];
			curr.pulse = pulse;
			insert_queue(&curr);
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
	queue = malloc(sizeof(signal) * QUEUE_SIZE);
	broadcaster = find("broadcaster");
	total_high_pulses = total_low_pulses = 0;
	for (i=0; i < 1000; i++)
		run(broadcaster);
	printf("%ld\n", ((long)total_high_pulses) * total_low_pulses);
	free(queue);
	free(line);
}
