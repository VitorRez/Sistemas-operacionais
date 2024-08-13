# _*_Makefile_*_ 
tp2virtual: tp2virtual.o page_table.o memory.o
	gcc -o tp2virtual tp2virtual.o page_table.o memory.o

page_table.o: page_table.c page_table.h
	gcc -c page_table.c

memory.o: memory.c memory.h
	gcc -c memory.c

tp2virtual.o: tp2virtual.c page_table.h
	gcc -c tp2virtual.c

clean:
	rm *.o tp2virtual