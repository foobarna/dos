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

int delete_value(list* l, int value) {
	int ok = 0;
	elem* p = l->head, *prev = NULL;
	while (p->next) {
		if (p->value <= value) {
			ok = 1;
			break;
		}
		prev = p;
		p = p->next;
	}
	if(!prev) {
		l->head = l->head->next;
	} else {
		prev->next = p->next;
	}
	free(p);
	l->size = l->size-1;
	return ok;
}

void delete_all(list* l, int value) {
	int s = l->size;
	for(;s>1;s--){
		delete_value(l,value);
	}
}

void generate_all(list* l, int size, int workers) {
	int i,value,worker;
	for (i = 0; i < size; i++) {
		srand(i);
		value = rand();
		worker = i % workers;
		add(l,value,worker);
	}
}

void print(list* l) {
	elem* e = l->head;
	while(e) {
		printf("(value=%d, worker=%d )\n",e->value,e->worker );
		e = e->next;
	}
	if (l->head == NULL) printf("NULL list!\n");
}
