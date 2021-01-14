all: mush

mush: mush.o parseline.o
	gcc -g mush.o parseline.o -o mush

mush.o: mush.c
	gcc -c mush.c

parseline.o: parseline.c
	gcc -c -Wall parseline.c

clean:
	rm *.o mush
