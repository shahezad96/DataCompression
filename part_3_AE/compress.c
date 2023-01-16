#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Int_Long_Pair
{
	unsigned int symbol;
	long count;
	long low;
} Int_Long_Pair;

 

int compare_count(const void *a,const void *b){
	Int_Long_Pair *x,*y;
	x =(Int_Long_Pair *)a;
	y =(Int_Long_Pair *)b;
	if(x->count > y->count){
		return -1;
	}else if(x->count < y->count){
		return 1;
	}
	return 0;
}

#define BUF_SIZE 1024


void count_freq(FILE *fp, Int_Long_Pair *char_freq){
	int i=0;
	unsigned char buffer[BUF_SIZE];
	fseek(fp,0,SEEK_SET);
	while(!feof(fp)){
		int char_count = fread(buffer,sizeof(char),BUF_SIZE,fp);
		for(i=0; i<char_count; i++){
			char_freq[buffer[i]].count++;
		}
	}
}


void write_bit(FILE *fp, char bit, int islast){
	static long bytes_count=0;
	static int bits_count=0;
	static unsigned long tmp=0;
	char buf_size = sizeof(unsigned long);
	//#define BUF_TYPE unsigned long
	tmp = (tmp<<1) | bit;
	bits_count++;
	
	if(islast){
		tmp = tmp << ((8*buf_size)-bits_count);
		bits_count = 8*buf_size;
	}
	if(bits_count == 8*buf_size ){
		fwrite(&tmp,sizeof(char),buf_size,fp);
		tmp = 0;
		bits_count=0;
		bytes_count = bits_count + buf_size;
	}
}

// read given file and encode and write to output file
void encode(FILE *fp,char *fname,long total,Int_Long_Pair *sym_freq,int sym_count){
	FILE *fout;
	char foutname[50];
	int i=0;
	unsigned char buffer[BUF_SIZE];
	unsigned char ch;
	unsigned long freq;
	
	
	typedef unsigned long ulong;
	ulong max = (~(ulong)0)-1;//0xFFFFFFFFFFFFFFFF-1;
	ulong _1by2 = (max)/2;
	ulong _1by4 = (max)/4;
	ulong _3by4 = (max/4)*3;
	ulong low = 0;
	ulong range = max;
	//ulong bitcount=0;
	long bitcount=0;
	

	strcpy(foutname,fname);
	strcat(foutname,".AE");
	
	fout = fopen(foutname,"wb");
	
	if(!fout){
		printf("Error : %s file not opening \n");
		exit(-1);
	}

	fseek(fp,0,SEEK_SET);
	int bits_used= 0;
	while(!feof(fp)){
		int char_count = fread(buffer,sizeof(char),BUF_SIZE,fp);
		for(i=0; i<char_count; i++){
			#define _ ;
			//#define _ printf
			// encoding is done here
			//printf("\nlow = %lu range = %lu \n",low,range);
			//printf("encoding sym = %d [%c] \n",buffer[i],buffer[i]);
			low = low +(range*sym_freq[buffer[i]].low);
			//range = range*(sym_freq[buffer[i]].count/(double)total);
			range = sym_freq[buffer[i]].count*(range/total);
			//getchar();
			while( low >= _1by2 || (low+range) <= _1by2){
				// our range is in left or right of _1by2
				bits_used += 1+bitcount;
				if(low>=_1by2){
					printf("1");
					write_bit(fout,1,0);
					while(bitcount>0){
						printf("0");
						write_bit(fout,0,0);
						--bitcount;
					}
					low = 2*(low-_1by2);
					range = 2*range;
				}else if((low+range) <= _1by2){
					printf("0");
					write_bit(fout,0,0);
					while(bitcount>0){
						printf("1");
						write_bit(fout,1,0);
						--bitcount;
					}
					low = 2*low;
					range = 2*range;
				}
				_("1by2      %lu\n",_1by2);
				_("low       %lu\n",low);
				_("low+range %lu\n",(low+range));
					
				//getchar();
				_("\nwhile 1 \n");
			}
			while(low > _1by4 && (low+range) < _3by4 ){
				bitcount++;
				low = 2*(low-_1by4);
				range = range*2;
				_("\nwhile 2 \n");
			}
		}
		// encode special symbol 256 as end of our data
		low = low +(range*sym_freq[256].low);
		//range = range*(sym_freq[buffer[i]].count/(double)total);
		range = sym_freq[256].count*(range/total);
		//getchar();
		while( low >= _1by2 || (low+range) <= _1by2){
			// our range is in left or right of _1by2
			bits_used += 1+bitcount;
			if(low>=_1by2){
				printf("1");
				write_bit(fout,1,0);
				while(bitcount>0){
					printf("0");
					write_bit(fout,0,0);
					--bitcount;
				}
				low = 2*(low-_1by2);
				range = 2*range;
			}else if((low+range) <= _1by2){
				printf("0");
				write_bit(fout,0,0);
				while(bitcount>0){
					printf("1");
					write_bit(fout,1,0);
					--bitcount;
				}
				low = 2*low;
				range = 2*range;
			}
			_("1by2      %lu\n",_1by2);
			_("low       %lu\n",low);
			_("low+range %lu\n",(low+range));
				
			//getchar();
			_("\nwhile 1 \n");
		}
		while(low > _1by4 && (low+range) < _3by4 ){
			bitcount++;
			low = 2*(low-_1by4);
			range = range*2;
			_("\nwhile 2 \n");
		}
		
		++bitcount;
		bits_used += 1+bitcount;
		if(low <= _1by4 ){
			printf("0");
			write_bit(fout,0,bitcount==0);
			while(bitcount>0){
				printf("1");
				write_bit(fout,1,bitcount==1);
				--bitcount;
			}
			range = range*2;
		}else{
			printf("1");
			write_bit(fout,1,bitcount==0);
			while(bitcount>0){
				printf("0");
				write_bit(fout,0,bitcount==1);
				--bitcount;
			}
			range = range*2;
		}
	}
	printf("\n bits used = %d \n",bits_used);
	fclose(fout);
}
	


int main(int argc, char **argv){
	FILE *fin;
	Int_Long_Pair sym_freq[257];
	int sym_count=257;
	long total;
	int i;
	
	if(argc<2){
		printf("usage: program [file]\n");
		return 0;
	}
	for(i=0;i<sym_count;i++){
		sym_freq[i].symbol=i;
		sym_freq[i].count=0;
		sym_freq[i].low=0;
	}
	
	sym_freq[256].symbol=256;
	sym_freq[256].count=1;
	fin = fopen(argv[1],"rb");
	
	if(!fin){
		printf("Error : %s file not opening \n");
		exit(-1);
	}
	fseek(fin,0,SEEK_END);
	total = ftell(fin)+1;
	
	count_freq(fin,sym_freq);
	Int_Long_Pair *tmp = (Int_Long_Pair *) malloc(sizeof(Int_Long_Pair)*sym_count);
	if(!tmp){
		printf("Error : memory not allocated \n");
		exit(-1);
	}
	for(i=0;i<sym_count;i++){
		tmp[i] = sym_freq[i];
	}
	qsort(tmp,sym_count,sizeof(Int_Long_Pair),compare_count);
	
	
	long count=0;
	for(i=0;i<sym_count;i++){
		sym_freq[tmp[i].symbol].low = count;
		count = count + tmp[i].count;
	}
	free(tmp);
	
	printf("Total symbol count = %ld \n",total);
	for(i=0;i<sym_count;i++){
		if(sym_freq[i].count==0)
			continue;
		printf("char = [ %c ]  symbol = %3d  low = %5d  count = %5d \n",sym_freq[i].symbol,sym_freq[i].symbol,sym_freq[i].low,sym_freq[i].count);
	}
	
	
	encode(fin,argv[1],total,sym_freq,sym_count);
	fclose(fin);
	
	return 0;
}