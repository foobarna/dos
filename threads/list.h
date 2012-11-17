typedef struct elem {
	int value;
	int worker;
	struct elem* next;
}elem;

typedef struct {
	elem* head;
	elem* last;
	int size;
}list;

void add(list* l, int value, int worker) {
	if (!l->head) {
		l->head = malloc(sizeof(elem));
		l->head->value = value;
		l->head->worker = worker;
		l->head->next = NULL;
		l->last = l->head;
		l->size = 1;
	} else {
		elem* e = malloc(sizeof(elem));
		e->value = value;
		e->worker = worker;
		e->next = NULL;
		l->last->next = e;
		l->last = e;
		l->size++;
	}
}

void generate_all(list* l, int size, int workers) {
	int i,value,worker;
	for (i = 0; i < size; i++) {
		srand(i);
		value = rand() % 10000;
		worker = rand() % (workers-1);
		add(l,value,worker);
	}
}

void print(list* l) {
	elem* e = l->head;
	while(e) {
		printf("(value=%d, worker=%d )\n",e->value,e->worker );
		e = e->next;
	}
}

// void delete(list* l, int value){
// 	elem* e = malloc(sizeof(elem));
// 	e = l->head;
// 	while (e->next) {
// 		if e->value < 2
// 	}
// }
