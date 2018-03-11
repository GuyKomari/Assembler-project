#	Authors: Gil Mansharov
#			 Guy Komari
#
#	File Name: Makefile
#
#	TODO: add "secondPass.o" (Responsible: GUY)


CFLAGS = -Wall -ansi -pedantic
CC = gcc

Assembler: main.o assembler.o globalVariables.o GlobalsFunctions.o \
	errors.o dataList.o symbolsList.o firstPass.o
	$(CC) $(CFLAGS) -o Assembler main.o assembler.o globalVariables.o GlobalsFunctions.o \
	errors.o dataList.o symbolsList.o firstPass.o


main.o: main.c main.h
	$(CC) $(CFLAGS) -c main.c


assembler.o: assembler.c assembler.h firstPass.h GlobalsFunctions.h errors.h
	$(CC) $(CFLAGS) -c assembler.c

GlobalsFunctions.o: GlobalsFunctions.c GlobalsFunctions.h dataList.h symbolsList.h globalVariables.h errors.h
	$(CC) $(CFLAGS) -c GlobalsFunctions.c


globalVariables.o: globalVariables.c globalVariables.h
	$(CC) $(CFLAGS) -c globalVariables.c


errors.o: errors.c errors.h
	$(CC) $(CFLAGS) -c errors.c


dataList.o: dataList.c dataList.h globalVariables.h
	$(CC) $(CFLAGS) -c dataList.c


symbolsList.o: symbolsList.c symbolsList.h globalVariables.h
	$(CC) $(CFLAGS) -c symbolsList.c


firstPass.o: firstPass.c firstPass.h GlobalsFunctions.h symbolsList.h errors.h
	$(CC) $(CFLAGS) -c firstPass.c


clean: Assembler
	-rm -f *.o