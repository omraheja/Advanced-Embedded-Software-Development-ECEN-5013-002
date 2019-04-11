# _*_ Makefile _*_

CC=arm-linux-gcc

OBJ = mypipe.o

mythread: $(OBJ)
	$(CC) -o mypipe $(OBJ)

mythread.o: mypipe.c
	$(CC) -c mypipe.c

clean:
	rm *.o mypipe
	rm om.txt
