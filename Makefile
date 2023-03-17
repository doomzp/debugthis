compiler_p = g++
compiler_t = gcc

main: debths.o
	$(compiler_t) -o testinf test/main.c
	$(compiler_p) -o debths debths.o
	./debths testinf
	rm debths testinf *.o

get_elf:
	$(compiler_t) -o testinf test/main.c
	readelf -h testinf > elf_infos/header
	readelf -S testinf > elf_infos/sections
	readelf -s testinf > elf_infos/symbols
	rm testinf
