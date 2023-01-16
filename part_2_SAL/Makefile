.SILENT: help

all: gen rec

gen: gen_access_pattern.c
	gcc -o gen gen_access_pattern.c

rec: recover_file_back.c
	gcc -o rec recover_file_back.c

debug: clean dgen drec

dgen: gen_access_pattern.c
	gcc -g -o gen gen_access_pattern.c

drec: expand.c
	gcc -g -o rec recover_file_back.c

rebuild: clean all


clean:
	rm -f gen
	rm -f rec

help:
	echo "----to build all (gen & rec) programs run----"
	echo "make all"
	echo "----to remove all executable files----"
	echo "make clean"
	echo "----to remove all executable files and rebuild----"
	echo "make rebuild"
	