#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
	unsigned char item;     // item
	struct Node *next; // Pointer to next node
} Node;

typedef struct LinkedList
{
	Node *head; // Pointer to first node of list
	int len;    // Length of linked list
} LinkedList;

////////////////////////////////////////////////////

/// You can add helper methods here
void printLL(LinkedList s)
{
	Node *tmp;
	tmp = s.head;
	printf("len=%d [ ", s.len);
	while (tmp != NULL)
	{
		printf("%u  ", tmp->item);
		tmp = tmp->next;
	}
	printf("] \n");
}

////////////////////////////////////////////////////

// Read 'len' number of integers and create LinkedList from it.
// Return address of the sequence.

LinkedList *build(int len)
{
	int i;
	LinkedList *ls;
	Node *tmp, *last;
	ls = malloc(sizeof(LinkedList));
	if (ls == NULL)
	{
		printf("memory allocation failed \n");
		return NULL;
	}
	ls->len = 0;
	ls->head = NULL;
	if(len==0)
		return ls;
	printf("Enter %d integers \n", len);
	for (i = 0; i < len; i++)
	{
		tmp = malloc(sizeof(Node));
		if (tmp == NULL)
		{
			printf("memory allocation failed \n");
			printf("only %d elements allocated out of %d \n", i + 1, len);
			return ls;
		}
		tmp->next = ls->head;
		ls->head = tmp;

		scanf("%d", &tmp->item);
		ls->len++;
	}
	return ls;
}

// Replace 'i'th node of the sequence with new item 'x'.
void set_at(int i, Node *x, LinkedList *s)
{
	int j;
	Node *tmp;
	tmp = s->head;
	for (j = 0; j < i - 1; j++)
	{
		if (tmp->next == NULL)
		{
			printf("index %d out of range \n", i);
			return;
		}
		tmp = tmp->next;
	}
	x->next = tmp->next->next;
	free(tmp->next);
	tmp->next = x;
}

// Return i th item
Node *get_at(int i, LinkedList s)
{
	int j;
	Node *tmp;
	tmp = s.head;
	for (j = 0; j < i; j++)
	{
		if (tmp->next == NULL)
		{
			printf("index %d out of range \n", i);
			return NULL;
		}
		tmp = tmp->next;
	}
	return tmp;
}

// Return Length of sequence
int length(LinkedList s)
{
	return s.len;
}

// insert 'x' at i th position in sequence s (REMEBER ith ITEM OF SEQUENCE NOT ARRAY).
void insert_at(int i, Node *x, LinkedList *s)
{
	int j;
	Node *tmp;
	tmp = s->head;
	for (j = 0; j < i - 1; j++)
	{
		if (tmp->next == NULL)
		{
			printf("index %d out of range \n", i);
			return;
		}
		tmp = tmp->next;
	}
	x->next = tmp->next;
	tmp->next = x;
	s->len++;
}

// removes the ith element of the sequence and returns the pointer to deleted item.
Node *delete_at(int i, LinkedList *s)
{
	int j;
	Node *tmp, *x;
	tmp = s->head;
	for (j = 0; j < i - 1; j++)
	{
		if (tmp->next == NULL)
		{
			printf("index %d out of range \n", i);
			return NULL;
		}
		tmp = tmp->next;
	}
	x = tmp->next;
	tmp->next = tmp->next->next;
	x->next = NULL;
	s->len--;
	return x;
}

// insert item 'x' at the start of the sequence.
void insert_first(Node *x, LinkedList *s)
{
	x->next = s->head;
	s->head = x;
	s->len++;
}
// insert item 'x' at the end of the sequence.
void insert_last(Node *x, LinkedList *s)
{
	Node *tmp;
	tmp = s->head;
	if (tmp == NULL)
	{
		s->head = x;
		return;
	}
	while (tmp->next != NULL)
	{
		tmp = tmp->next;
	}
	tmp->next = x;
	while (tmp->next != NULL)
	{
		// if x is not single Node but chain of nodes
		// increament for each node
		s->len++;
		tmp = tmp->next;
	}
}

// delete first item of sequence and return pointer to deleted item.
Node *delete_first(LinkedList *s)
{
	Node *x;
	x = s->head;
	s->head = s->head->next;
	s->len--;
	return x;
}

// delete last item of sequence and return pointer to deleted item.
Node *delete_last(LinkedList *s)
{
	Node *tmp, *x;
	tmp = s->head;
	// empty list
	if (s->head == NULL)
		return NULL;
	// just one element
	if (s->head->next == NULL)
	{
		x = s->head;
		s->head = NULL;
		s->len--;
		return x;
	}
	while (tmp->next->next != NULL)
	{
		tmp = tmp->next;
	}
	x = tmp->next;
	tmp->next = NULL;
	s->len--;
	return x;
}

Node *get_last(LinkedList s){
	Node *tmp;
	tmp = s.head;
	// empty list
	if (tmp == NULL)
		return NULL;
	// just one element
	while(tmp->next != NULL){
		tmp = tmp->next;
	}
	return tmp;
}

Node *create_node(unsigned char c){
	Node *node;
	node = malloc(sizeof(Node));
	if(node==NULL){
		printf("Error in allocating memory \n");
		exit(1);
	}
	node->next = NULL;
	node->item = c;
	return node;
}



#define BITS_4 4
#define BUF_SIZE 65536

void move_to_front(char *array, int n, int index){
	int tmp;
	int i = index;
	while(i>0){
		tmp = array[i-1];
		array[i-1] = array[i];
		array[i] = tmp;
		i--;
	}
}

void move_left(char *array, int n, int index){
	int tmp;
	int i = index;
	if(i>0){
		tmp = array[i-1];
		array[i-1] = array[i];
		array[i] = tmp;
		i--;
	}
}


unsigned char get_access(LinkedList *ls, unsigned char value){
	int i=value;
	Node *tmp = ls->head;
	Node *tmp_prev = NULL;
	if(!tmp){
		printf("Error Empty list\n");
		exit(1);
	}
	if(i<0)printf("error %d\n",i);
	while(i>0){
		tmp_prev = tmp;
		tmp = tmp->next;
		i--;
	}
	if(tmp_prev){
		tmp_prev->next = tmp->next; // remove matching element (tmp) from list
		
		tmp->next = ls->head;  // add tmp2 before head
		ls->head = tmp;		// make tmp2 new head of ls
	}
	return tmp->item;
}


int main(int argc, char **argv){
	FILE *fin,*fout;
	char fname[50];
	unsigned char buffer[BUF_SIZE];
	LinkedList *ls;
	int i;
	
	
	if(argc<2){
		printf("usage: program [file]\n");
		return 0;
	}
	
	ls = build(0);
	for(i=255;i>=0;i--){
		insert_first(create_node(i),ls);
	}
	
	fin = fopen(argv[1],"rb");
	strcpy(fname,"org_rec_");
	strcat(fname,&argv[1][3]);
	fout = fopen(fname,"w+b");
	
	while(!feof(fin)){
		int char_count = fread(buffer,sizeof(char),BUF_SIZE,fin);
		i = 0;
		//printf("char_count=%d\n",char_count);
		for(int i=0; i<char_count; i++){
			buffer[i] = get_access(ls,buffer[i]);
		}
		fwrite(buffer,sizeof(char),char_count,fout);
		fflush(fout);
	}
	return 0;
}