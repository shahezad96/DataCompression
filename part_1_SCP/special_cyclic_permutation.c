#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linkedlist.h"



int compare(long a, long b,unsigned char *file_content, long fsize, long level){
	unsigned char x,y;
	long i=0;
	int j;
	for(i=level;i<fsize;i++){
		//printf("comparing [%d] %c [%d] %c \n",a,x[j],b,y[j]);
		x = file_content[(a+i)%fsize];
		y = file_content[(b+i)%fsize];
		if( x>y )
			return 1;
		else if(x<y)
			return -1;
	}
	return 0;
}


void swap(long a, long b, long *index_array){
	//printf("swaping %ld with %ld \n",a,b);
	long tmp = index_array[a];
	index_array[a] = index_array[b];
	index_array[b] = tmp;
}

long *aux;

///*
void merge(long *arr1, long size1, long *arr2, long size2,unsigned char *file_content,long fsize, long level){
	long i,j,k;
	i=j=k=0;
	char value;
	long *result;
	result = aux; //malloc(sizeof(long)*(size1+size2));
	
	while(i<size1 && j<size2){
		value = compare(arr1[i],arr2[j],file_content,fsize,level);
		if(value<=0){
			result[k] = arr1[i];
			i++;k++;
		}else{
			result[k] = arr2[j];
			j++;k++;
		}
	}
	while(i<size1){
		result[k] = arr1[i];
		i++;k++;
	}
	while(j<size2){
		result[k] = arr2[j];
		j++;k++;
	}
	
	
	///*
	// copy back into orignal
	i=0;
	while(i<size1){
		arr1[i] = result[i];
		i++;
	}
	j=0;
	while(j<size2){
		arr2[j] = result[i+j];
		j++;
	}
	//*/
	//free(result);
}

///*
void merge_sort(unsigned char *file_content,long *index_array, long fsize,long asize,long level){
	long i,j;
	int value;
	long mid;
	long *result;
	
	if(asize==1)
		return;
	mid = asize/2;
	merge_sort(file_content,index_array,fsize,mid,level);
	merge_sort(file_content,index_array+mid,fsize,asize-mid,level);
	merge(index_array,mid,index_array+mid,asize-mid,file_content,fsize,level);
	return;
}
//*/

///*
// multithreaded merge_sort
#include <pthread.h>

typedef struct Args{
	unsigned char *file_content;
	long *index_array;
	long fsize;
	long asize;
	long level;
}Args;

void merge_sort_mth(unsigned char *file_content,long *index_array, long fsize,long asize,long level);

int threads = 0;

void *merge_sort_mth_helper(void *_args){
	Args *args = (Args *)_args;
	unsigned char *file_content = args->file_content;
	long *index_array = args->index_array;
	long fsize = args->fsize;
	long asize = args->asize;
	long level = args->level;
	
	long i,j;
	int value;
	long mid;
	long *result;
	
	if(asize==1)
		return NULL;
	mid = asize/2;
	
	Args tmp;
	tmp.file_content = file_content;
	tmp.index_array = index_array;
	tmp.fsize = fsize;
	tmp.asize = mid;
	tmp.level = level;
	
	pthread_t id;
	if(asize>256 && threads<100){
		pthread_create(&id, NULL, merge_sort_mth_helper, (void *)&tmp);
		threads++;
		//merge_sort(file_content,index_array,fsize,mid,level);
		merge_sort_mth(file_content,index_array+mid,fsize,asize-mid,level);
		pthread_join(id, NULL);
		threads--;
	}else{
		merge_sort_mth(file_content,index_array,fsize,mid,level);
		merge_sort_mth(file_content,index_array+mid,fsize,asize-mid,level);
	}
	merge(index_array,mid,index_array+mid,asize-mid,file_content,fsize,level);
	return NULL;
}

void merge_sort_mth(unsigned char *file_content,long *index_array, long fsize,long asize,long level){
	long i,j;
	int value;
	long mid;
	long *result;
	
	if(asize==1)
		return;
	mid = asize/2;
	

	Args tmp;
	tmp.file_content = file_content;
	tmp.index_array = index_array;
	tmp.fsize = fsize;
	tmp.asize = mid;
	tmp.level = level;
	
	pthread_t id;
	if( asize>256 && threads<100){
		pthread_create(&id, NULL, merge_sort_mth_helper, (void *)&tmp);
		threads++;
		//merge_sort(file_content,index_array,fsize,mid,level);
		merge_sort_mth(file_content,index_array+mid,fsize,asize-mid,level);
		pthread_join(id, NULL);
		threads--;
	}else{
		merge_sort_mth(file_content,index_array,fsize,mid,level);
		merge_sort_mth(file_content,index_array+mid,fsize,asize-mid,level);
	}
	merge(index_array,mid,index_array+mid,asize-mid,file_content,fsize,level);
	return;
}

// multithreaded merge_sort end 
//*/



long *base;
long sorted_count = 0 ;


void radix_sort(unsigned char *file_content, long *index_array, long fsize,long asize, long level){
	//printf("radix_sort(asize=%ld level=%ld index_array=0x%x)\n",asize,level,index_array);
	long i,j,size;
	unsigned char ch;
	LinkedList *ls_array;
	Node *tmp;
	if(level==0)
		base = index_array;
	///*
	if((index_array-base)>sorted_count+10240){
		printf("%ld KB done \n",(index_array-base)/1024);
		sorted_count = (index_array-base);
	}
	//*/
	//printf("%ld Bytes done \n",(index_array-base));
	//sorted_count = (index_array-base);
	if(asize==1)
		return;
	if(asize > 256){
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
			ch = file_content[(index_array[i]+level)%fsize];
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
				//printf("j-size = %d \n",j-size);
				//if(level==0)
				//	printf("sorting %ld elements together \n",size);
				///*
				
				// only go upto this level
				if(level>fsize){
					merge_sort(file_content,(index_array+j-size),fsize,size,level);
					continue;
				}
				
				// if more than half of the nodes are in one linkedlist out of 256 
				if(size > asize*0.50){
					//printf("%d condition success \n",size);
					long mid = size/2;
					radix_sort(file_content, (index_array+j-size), fsize, mid, level+1);
					radix_sort(file_content, (index_array+j-(size-mid)), fsize, size-mid, level+1);
					merge((index_array+j-size), mid, (index_array+j-(size-mid)), size-mid,file_content,fsize,level+1);
					continue;
				}
				//*/
				radix_sort(file_content, (index_array+j-size), fsize, size, level+1);
				
				//if(level==0)
				//	printf("%d KB done \n",j/1024);
			}
		}
		free(ls_array);
	}else{
		merge_sort(file_content,index_array,fsize,asize,level);
		//merge_sort_mth(file_content,index_array,fsize,asize,level);
		return;
		
		/*
		// some other sort (insertion sort)
		int value;
		long a,b;
		for(i=0;i<asize;i++){
			for(j=i;j>0;j--){
				a = index_array[j-1];
				b = index_array[j];
				//printf("comparing [%d]=%c [%d]=%c \n",j-1,get_byte(fp,a,fsize),j,get_byte(fp,b,fsize));
				value = compare2(a,b,file_content,fsize,level);
				if(value>0){
					//printf("swaping [%d]=%c [%d]=%c \n",j-1,get_byte(fp,a,fsize),j,get_byte(fp,b,fsize));
					swap(j-1,j,index_array);
				}else{
					//printf("else [%d]=%c [%d]=%c \n",j-1,get_byte(fp,a,fsize),j,get_byte(fp,b,fsize));
					break;
				}
			}
		}
		//*/
	}
}




int main(int argc, char **argv){
	char in_fname[50],out_fname[50];
	FILE *infp,*lcfp; // infp: input file   lcfp:last column file pointer
	long *index_array, fsize,i,index,org_index;
	unsigned char ch;
	unsigned char *file_content;
	
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
	fsize = ftell(infp);
	index_array = malloc(sizeof(long)*fsize);
	if(index_array==NULL){
		printf("Memory Allocation Error \n");
		return 0;
	}
	file_content = malloc(sizeof(char)*fsize);
	if(file_content==NULL){
		printf("Memory Allocation Error \n");
		return 0;
	}
	
	aux = malloc(sizeof(long)*fsize);
	if(aux==NULL){
		printf("Memory Allocation Error \n");
		return 0;
	}
	
	fseek(infp,0,SEEK_SET);
	
	for(i=0;i<fsize;i++){
		fread(file_content+i,1,sizeof(unsigned char),infp);
		index_array[i] = i;
	}
	fclose(infp);
	
	printf("sorting \n");
	//sort(file_content,index_array,fsize);
	
	radix_sort(file_content,index_array,fsize,fsize,0);
	//radix_sort2(file_content,index_array,fsize,fsize,0);
	
	strcpy(out_fname,"lc_");
	strcat(out_fname,argv[1]);
	
	lcfp = fopen(out_fname,"wb");
	if(lcfp==NULL){
		printf("error: file %s not opening\n",out_fname);
		return 0;
	}
	
	fwrite(&org_index,1,sizeof(long),lcfp);
	
	for(i=0;i<fsize;i++){
		if(index_array[i]==0)
			org_index = i;
		index = (index_array[i]+fsize-1)%fsize;
		
		ch = file_content[index];
		fwrite(&ch,1,sizeof(char),lcfp);
	}
	free(index_array);
	free(file_content);
	fseek(lcfp,0,SEEK_SET);
	fwrite(&org_index,1,sizeof(long),lcfp);
	
	fclose(lcfp);
	return 0;
}