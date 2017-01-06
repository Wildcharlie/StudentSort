#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Double Linked List

typedef struct node {
	struct node *next;
	struct node *prev;
	void *val;
} node;

typedef struct List {
	node *head;
	node *last;
	int count;
} List;

List *createList() {
	return calloc(1, sizeof(List));
}

void insert(List *list, void *val) {
	node *x = calloc(1, sizeof(node));

	x->val = val;

	if (list->last == NULL) {
		list->head = x;
		list->last = x;
	}
	else {
		list->last->next = x;
		x->prev = list->last;
		list->last = x;
	}
	list->count+=1;
}

// Person Structure
struct person {
	int studentID;
	char *firstName;
	char *lastName;
	char *dept;
	float GPA;
};

struct person *createPerson(int id, char *first, char *last, char *dept, float GPA) {
	struct person *x = malloc(sizeof(struct person));
	assert(x!=NULL);

	x->studentID = id;
	x->firstName = strdup(first);
	x->lastName = strdup(last);
	x->dept = strdup(dept);
	x->GPA = GPA;

	return x;
}

void printPerson(struct person *x) {
	printf("ID: %d\n", x->studentID);
	printf("First: %s\n", x->firstName);
	printf("Last: %s\n", x->lastName);
	printf("Dept: %s\n", x->dept);
	printf("GPA: %.2f\n", x->GPA);
}

void deletePerson(struct person *x) {
	assert(x!=NULL);

	free(x->firstName);
	free(x->lastName);
	free(x->dept);
	free(x);
}


void deleteList(List *list) {
	node *y = list->head;
	for (; y != NULL; y = y->next) {
		if (y->prev) {		
			deletePerson(y->prev->val);
			free(y->prev);
		}
	}
	if (list->last != NULL) deletePerson(list->last->val);
	free(list->last);
	free(list);

}

void printList(List *list) {
	node *y = list->head;
	for (; y != NULL; y = y->next) {
		printPerson(y->val);

	}
}



// Swap Node with previous Node
// Current node = Third, Previous = second
void swap(List *list, struct node *third) {

	if (third != list->head) {
		struct node *first, *second, *fourth;

		if (third->prev->prev != NULL) first = third->prev->prev;
		second = third->prev;
		if (third != list->last) fourth = third->next;

		if (third->prev->prev != NULL) first->next = third;
		second->prev = third;
		if (third != list->last) second->next = fourth;
		if (second != list->head) third->prev = first;
		else {
			list->head = third;
			third->prev = NULL;
		}
		third->next = second;
		if (third != list->last) fourth->prev = second;
		else {
			list->last = second;
			second->next = NULL;
		}
	}
}


// Insertion Sort
int cmp(struct person *a, struct person *b) {
	if (a->studentID > b->studentID) return 0;
	else return 1;
}

void sort(List *list) {
	node *y = list->head->next;
	for (; y != NULL; y = y->next) {
		node *z = y;
		for (; z != list->head;) {
			if (cmp(z->val, z->prev->val)) swap(list, z);
			else break;
		}
	}
}


int main(int argc, char *argv[]) {

	// Initialize variables
	int id = 0, donereading = 0;
	char *first, *last, *dept;
	float gpa;

	FILE *fwrite, *fread;
	fread = fopen( argv[1], "r");

	List *list = createList();


	// Read from in file
	if (fread != NULL) {

		while (!donereading) {
			fscanf(fread, "%d %ms %ms %ms %f", &id, &first, &last, &dept, &gpa);
			if ((feof(fread)) || (ferror(fread))) donereading = 1;
			else {
				insert(list, createPerson(id, first, last, dept, gpa));
				free(first);
				free(last);
				free(dept);
			}	
		}
	}
	else {
		printf("\nError loading.\n");
		deleteList(list);
		exit(0);
	}

	fclose(fread);

	// Sort //
	sort(list);

	// Write to out file
	fwrite = fopen(argv[2], "w");

	if (fwrite != NULL) {
		node *y = list->head;
		for (; y != NULL; y = y->next) {
			if (y) {	
				fprintf(fwrite, "%d,%s,%s,%s,%.2f\n", ((struct person*)y->val)->studentID, ((struct person*)y->val)->firstName, ((struct person*)y->val)->lastName, ((struct person*)y->val)->dept, ((struct person*)y->val)->GPA);
			}
		}
		

	}
	else {
		printf("\nError saving.\n");
		deleteList(list);
		fclose(fwrite);
		exit(0);
	}

	fclose(fwrite);

	// Free memory
	deleteList(list);

	printf("Successfully saved.\n");

	return 0;

}
