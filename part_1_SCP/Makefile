.SILENT: help

all: scp scp_rec

scp: special_cyclic_permutation.c
	gcc -o scp special_cyclic_permutation.c

scp_rec: special_cyclic_permutation_recover.c
	gcc -o scp_rec special_cyclic_permutation_recover.c

dscp: special_cyclic_permutation.c
	gcc -g -o scp special_cyclic_permutation.c

dscp_rec: special_cyclic_permutation_recover.c
	gcc -g -o scp_rec special_cyclic_permutation_recover.c
	
debug: clean dscp dscp_rec

rebuild: clean all


clean:
	rm -f scp
	rm -f scp_rec

help:
	echo "----to build all (scp & scp_rec) programs run----"
	echo "make all"
	echo "----to remove all executable files----"
	echo "make clean"
	echo "----to remove all executable files and rebuild----"
	echo "make rebuild"
	