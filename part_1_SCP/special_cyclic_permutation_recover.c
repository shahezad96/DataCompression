#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linkedlist.h"

void swap(long a, long b, long *index_array){
	//printf("swaping %ld with %ld \n",a,b);
	long tmp = index_array[a];
	index_array[a] = index_array[b];
	index_array[b] = tmp;
}


// these are used for printing how many bytes are done sorting
long *base;
long sorted_count = 0 ;

void radix_sort(unsigned char *file_content, long *index_array, long fsize,long asize){
	//printf("radix_sort(asize=%ld level=%ld index_array=0x%x)\n",asize,level,index_array);
	long i,j,size;
	unsigned char ch;
	LinkedList *ls_array;
	Node *tmp;
	ls_array = malloc(sizeof(LinkedList)*256);
	if(ls_array==NULL){
		printf("Memory Allocation Error \n");
		return;
	}
	for(i=0;i<256;i++){
	  ls_array[i].len = 0;
	  ls_array[i].head=NULL;
	  ls_array[i].last=NULL;
	}
	for(i=0;i<asize;i++){
		ch = file_content[(index_array[i])%fsize];
		insert_last(create_node(index_array[i]),&ls_array[ch]);
	}
	j=0;
	for(i=0;i<256;i++){
		size = ls_array[i].len;
		if(size>0){
			tmp = delete_first(&ls_array[i]);
			while(tmp){
				index_array[j] = tmp->item;
				free(tmp);
				j++;
				tmp = delete_first(&ls_array[i]);
			}
		}
	}
}


int main(int argc, char **argv){
	char in_fname[50],out_fname[50];
	FILE *infp,*outfp;
	long *index_array, fsize,i,index,org_index;
	unsigned char ch;
	unsigned char *last_column;
	
	if(argc<2){
		printf("usage: program [filename]\n");
		return 0;
	}
	
	
	infp = fopen(argv[1],"rb");
	if(infp==NULL){
		printf("error: file %s not opening\n",argv[1]);
		return 0;
	}
	
	fseek(infp,0,SEEK_END);
	fsize = ftell(infp)-sizeof(long);
	index_array = malloc(sizeof(long)*fsize);
	if(index_array==NULL){
		printf("Memory Allocation Error \n");
		return 0;
	}
	last_column = malloc(sizeof(char)*fsize);
	if(last_column==NULL){
		printf("Memory Allocation Error \n");
		return 0;
	}
	fseek(infp,0,SEEK_SET);
	fread(&org_index,1,sizeof(long),infp);
	
	for(i=0;i<fsize;i++){
		fread(last_column+i,1,sizeof(unsigned char),infp);
		index_array[i] = i;
	}
	fclose(infp);
	
	printf("sorting \n");
	//sort(last_column,index_array,fsize);
	
	radix_sort(last_column,index_array,fsize,fsize);
	
	///*
	strcpy(out_fname,"orig_");
	strcat(out_fname,argv[1]+3); // remove lc_ prefix
	
	outfp = fopen(out_fname,"wb");
	if(outfp==NULL){
		printf("error: file %s not opening\n",out_fname);
		return 0;
	}
	
	index = index_array[org_index];
	for(i=0;i<fsize;i++){
		fwrite(&last_column[index],1,sizeof(char),outfp);
		index = index_array[index];
	}
	
	//*/
	
	fclose(outfp);
	return 0;
}