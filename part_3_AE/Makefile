.SILENT: help
help:
	echo "----to build all (com & exp) programs ----"
	echo "make all"
	echo "----to build all (com & exp) programs with -g switch ----"
	echo "make debug"
	echo "----to remove all executable files----"
	echo "make clean"
	echo "----to remove all executable files and rebuild----"
	echo "make rebuild"

all: com exp

com: compress.c
	gcc -o com compress.c

exp: expand.c
	gcc -o exp expand.c

debug: clean dcom dexp

dcom: compress.c
	gcc -g -o com compress.c

dexp: expand.c
	gcc -g -o rec expand.c

rebuild: clean all


clean:
	rm -f com
	rm -f exp

